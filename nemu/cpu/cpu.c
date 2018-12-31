#include "cpu/reg.h"
#include "cpu/mm.h"
#include "cpu/io.h"
#include "cpu/int.h"
#include "device/terminal.h"
#include "device/serial.h"
#include "device/ide.h"
#include "device/timer.h"
#include "device/keyboard.h"
#include "device/vga.h"
#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "ui/watchpoint.h"

// The start address of 'loader' is at 0x100000
extern uint8_t *hw_mem;
#define hwa_to_va(p) ((void *)(hw_mem + (unsigned)p))
#define LOADER_START 0x100000
extern uint8_t loader [];
extern uint32_t loader_len;
void load_prog();
void init_dram();
void init_cache();
void init_cache_L2();
void restart()
{
	// Perform some initialization to restart a program
	memcpy(hwa_to_va(LOADER_START), loader, loader_len);

	cpu.eip = LOADER_START;
	cpu.ebp = 0x00000000;
	cpu.esp = 0x08000000;
	cpu.eflags = 0x00000002;
	
	cpu.CR0_PE = 0;
	cpu.CR0_PG = 0;
	// index: 1, TI: 0, RPL: 0
	cpu.CS = 0x0008;

	init_dram();
	init_cache();
	init_cache_L2();
	tlb_flush();

	// register devices: serial/ide/timer/keyboard/vga
	serial_register();
	ide_register();
	timer_register();
	keyboard_register();
	vga_register();	
	terminal_clear();
}

extern int enable_debug;
extern int quiet;
char assembly[40];
static void print_instr(swaddr_t eip, int len);

int exec(swaddr_t);
void cpu_exec(volatile uint32_t n) 
{
	volatile uint32_t n_temp = n;

	while(n -- )
  	{
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);

 	 	if(n_temp != -1 || (enable_debug && !quiet)) 
			print_instr(eip_temp, instr_len);

		cpu.eip += instr_len;

		switch (bp_state) 
		{
			case OFF: 
				break;

			case TRIG:
				--cpu.eip;
				uint8_t instr = search_bp(cpu.eip);
				swaddr_write(cpu.eip, 1, instr);
	     		printf("Breakpoint at 0x%08X\n", cpu.eip);
		
				bp_state = RESET;
						
				return; 

			case RESET:
				swaddr_write(eip_temp, 1, INT3_CODE);
				bp_state = OFF;
				break;
		} 
			
		if (WP_STATE) 
		{ 
			bool changed = test_wp();
			if (changed) { nemu_state = STOP; return; }
		}

		if (cpu.INTR & cpu.IF )
		{
			uint8_t irqno;
			i8259_irq_query(&irqno);
			i8259_irq_ack();
			int_handle(irqno);
		}

		if (n == 0)  nemu_state = n_temp == -1 ? STOP : INT;
		switch(nemu_state)
		{
			case RUNNING :  continue;;
			case END     :  return;
			case STOP    :  return;
			case INT     :  return;
		}
	} 
}

uint32_t instr_fetch(swaddr_t eip, size_t len);
static void print_instr(swaddr_t eip, int len) 
{
	printf("  0x%.8x:   ", eip);

	int i;
	for(i = 0; i < len; i ++) 
		printf("%.2x ", instr_fetch(eip + i, 1));

	printf("%*.s", 50 - (12 + 3 * len), "");

	puts(assembly);
}

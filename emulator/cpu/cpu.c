#include "loader.h"
#include "cpu/reg.h"
#include "cpu/mm.h"
#include "cpu/io.h"
#include "cpu/int.h"
#include "ram/ram.h"
#include "device/terminal.h"
#include "device/serial.h"
#include "device/ide.h"
#include "device/timer.h"
#include "device/keyboard.h"
#include "device/vga.h"
#include "debugger/breakpoint.h"
#include "debugger/watchpoint.h"

#define LOADER_START 0x100000

void restart()
{
	memcpy(phy_mem + LOADER_START, loader, loader_len);

	cpu.eip = LOADER_START;
	cpu.ebp = 0x00000000;
	cpu.esp = 0x08000000;
	cpu.eflags = 0x00000002;	
	cpu.CR0_PE = 0;
	cpu.CR0_PG = 0;
	cpu.CS = 0x0008;

	flush_tlb();
	flush_cache();
	flush_l2cache();
	flush_dram();

	serial_register();
	ide_register();
	timer_register();
	keyboard_register();
	vga_register();	
	terminal_clear();
}

char assembly[40];
static void print_instr(swaddr_t eip, int len) 
{
	printf("  0x%.8x:   ", eip);

	int i;
	for(i = 0; i < len; i ++) 
		printf("%.2x ", instr_fetch(eip + i, 1));

	printf("%*.s", 50 - (12 + 3 * len), "");

	puts(assembly);
}

int exec(swaddr_t);
void cpu_exec(volatile uint32_t n) 
{
	volatile uint32_t n_temp = n;

	while (n--)
  	{
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);

		if(n_temp != -1 || emulator_enable_debug)
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

		if (n == 0)
			nemu_state = n_temp == -1 ? STOP : INT;

		switch(nemu_state)
		{
			case RUNNING :  continue;;
			case END     :  return;
			case STOP    :  return;
			case INT     :  return;
		}
	} 
}


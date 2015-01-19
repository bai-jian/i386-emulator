#include "nemu.h"

#include "device/i8259.h"

#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "ui/watchpoint.h"


// The start address of 'loader' is at 0x100000
// The start address of 'user program' is at 0x800000
#define LOADER_START 0x100000
extern uint8_t loader [];
extern uint32_t loader_len;
void load_prog();
void init_dram();
void init_cache();
void init_cache_L2();
void init_TLB();
void sdl_clear_event_queue();
void restart() 
{
	// Perform some initialization to restart a program
//  load_prog();
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
	init_TLB();

	sdl_clear_event_queue();
}


char assembly[40];
extern int enable_debug;
extern int quiet;
int exec(swaddr_t);
static void print_bin_instr(swaddr_t, int);
void raise_intr(uint8_t);
void cpu_exec(volatile uint32_t n) 
{
	volatile uint32_t n_temp = n;

	while(n -- )
  	{
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);

 	 	if(n_temp != -1 || (enable_debug && !quiet)) 
		{
			// print the instruction executed
			print_bin_instr(eip_temp, instr_len);
			puts(assembly);
		} 

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
			uint8_t intr_no = i8259_query_intr();
			i8259_ack_intr();
			raise_intr(intr_no);
		}

		switch(nemu_state)
		{
			case END:
				return;

			case RUNNING:
				if (n == 0)  { nemu_state = STOP;  return;  }
		 }
	} 
}

uint32_t instr_fetch(swaddr_t eip, size_t len);
static void print_bin_instr(swaddr_t eip, int len) 
{
	printf("  0x%.8x:   ", eip);

	int i;
	for(i = 0; i < len; i ++) 
		printf("%.2x ", instr_fetch(eip + i, 1));

	printf("%*.s", 50 - (12 + 3 * len), "");
}

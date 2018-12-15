#include "helper.h"
#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "nemu.h"

// invalid opcode
make_helper(inv)
{
	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
	assert(0);
}

make_helper(int3)
{
	nemu_state = INT;
	bp_state = TRIG;
	return 1;
}


enum  {  TRAP_good, TRAP_bad, TRAP_stop  };
static void  hit_trap_good ();
static void  hit_trap_bad  ();
static void  hit_trap_stop ();
make_helper(nemu_trap)
{ 
	switch( cpu.eax )
 	{
		case TRAP_good	:	hit_trap_good();	break;
		case TRAP_bad 	:	hit_trap_bad();		break;
		case TRAP_stop	:	hit_trap_stop();	break;
		default			:	assert(0);
	}
	print_asm("nemu trap");
	return 1;
}

static void hit_trap_good()
{
	printf("\nnemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", "GOOD", cpu.eip);
	nemu_state = END;
}

static void hit_trap_bad()
{	
	printf("\nnemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", "BAD", cpu.eip);
	nemu_state = END;
}

static void hit_trap_stop()
{
	printf("\nnemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x :  Kernel has been restarted.\n", "STOP", cpu.eip);
	puts  ("(Use 'c' to continue the user program.)\n");
	nemu_state = STOP;
}

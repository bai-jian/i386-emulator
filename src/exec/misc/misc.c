#include "exec/helper.h"

#include "ui/ui.h"
#include "ui/breakpoint.h"

#include "nemu.h"

make_helper(inv) {
	/* invalid opcode */

	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);

	assert(0);
}

make_helper(int3)
{
	bp_state = TRIG;
	return 1;
}

make_helper(nemu_trap)
{
	switch( cpu.eax )
	{
		case 0:
		{
			printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", "GOOD", cpu.eip);
			nemu_state = END;

			print_asm("nemu trap");

			return 1;
		}
		case 1:
		{	
			printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", "BAD", cpu.eip);
			nemu_state = END;

			print_asm("nemu trap");

			return 1;
		}
		case 2:  // SYS_write
		{
			char buffer[1024];

			uint32_t buf = cpu.ecx;
			uint32_t len = cpu.edx;

			int i;
			for (i = 0; i < len; ++ i)
				buffer[i] = swaddr_read(buf + i, 1);
			printf("%s", buffer);
			
			return 1;
		}
		default:
		{	assert(0);  return 0;    }
	}
}

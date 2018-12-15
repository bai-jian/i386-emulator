#include "helper.h"
#include "cpu/reg.h"
#include "common.h"

make_helper(hlt)
{
	//while(!(cpu.IF && cpu.INTR)) ;
	volatile bool *p = &cpu.INTR;
	if (cpu.IF)
		while(*p == 0);
	return 1;
}

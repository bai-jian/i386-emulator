#include "exec/helper.h"

make_helper(hlt)
{
	//while(!(cpu.IF && cpu.INTR)) ;
	return 1;
}

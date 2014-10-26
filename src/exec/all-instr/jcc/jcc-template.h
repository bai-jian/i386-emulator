#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper( concat(je_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("je" " %x", eip);

	return 1 + DATA_BYTE + disp;
}
	

#include "exec/template-end.h"

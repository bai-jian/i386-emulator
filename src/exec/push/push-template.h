#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper( concat(push_r_, SUFFIX) )
{
	uint8_t reg_code = instr_fetch(eip, 1) & 0x07;

	cpu.esp -= DATA_BYTE;
	MEM_W( cpu.esp, REG(reg_code) );
	
	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));

	return 1;
}

#include "exec/template-end.h"

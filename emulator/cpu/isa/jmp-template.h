#include "template-start.h"
#include "modrm.h"

//jmp   rel8/rel16/rel32
make_helper( concat(jmp_i_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;

	int32_t imm = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += imm;

	print_asm("jmp    $0x%x", imm);

	return instr_len;
}


#include "template-end.h"

#include "exec/helper.h"


#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE


extern char suffix;

make_helper(call_i_v)
{
	if (suffix == 'l')
		return call_i_l(eip);
	else
	{
		uint8_t instr_len = call_i_w(eip);
		cpu.eip = cpu.eip & 0x0000FFFF;
		return instr_len;
	}
}

make_helper(call_rm_v)
{
	if (suffix == 'l')
		return call_rm_l(eip);
	else
	{
		uint8_t instr_len = call_rm_w(eip);
		cpu.eip = cpu.eip & 0x0000FFFF;
		return instr_len;
	}
}

#include "exec/helper.h"

#define  DATA_BYTE 1
#include "jmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "jmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "jmp-template.h"
#undef   DATA_BYTE


#include "cpu/reg.h"

extern char suffix;

make_helper(jmp_i_v)
{	
	if (suffix == 'l')
		return jmp_i_l(eip);
	else
	{
		cpu.eip = cpu.eip & 0x0000FFFF;
		return jmp_i_w(eip);
	}
}

make_helper(jmp_rm_v)
{	
	if (suffix == 'l')
		return jmp_rm_l(eip);
	else
	{
		cpu.eip = cpu.eip & 0x0000FFFF;
		return jmp_rm_w(eip);
	}
}

make_helper(jmp_seg_v)
{	
	if (suffix == 'l')
		return jmp_seg_l(eip);
	else
	{
		cpu.eip = cpu.eip & 0x0000FFFF;
		return jmp_seg_w(eip);
	}
}

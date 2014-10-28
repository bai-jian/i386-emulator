#include "exec/template-start.h"


#include "cpu/modrm.h"
#include "exec/set_eflags/set_eflags.h"

make_helper( concat(cmp_i2r0_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip+1, DATA_BYTE);
	DATA_TYPE rva = REG(0);

	concat(set_CF_, SUFFIX) (rva, imm, 1);
	concat(set_SF_, SUFFIX) (rva, imm, 1);
	concat(set_PF_, SUFFIX) (rva, imm, 1);
	concat(set_ZF_, SUFFIX) (rva, imm, 1);
	concat(set_OF_, SUFFIX) (rva, imm, 1);

	print_asm("cmp" str(SUFFIX) " 0x%x,%%%s", imm, REG_NAME(0));

	return 1 + DATA_BYTE;
}

make_helper( concat(cmp_i2rm_, SUFFIX) )
{ 
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.reg == 7)
 	{
		uint8_t imm_byte = DATA_BYTE;
		if ( instr_fetch(eip, 1) == 0x83 ) imm_byte = 1;

		if (m.mod != 3)
		{
			swaddr_t  addr;
			uint8_t   len = read_ModR_M(eip+1, &addr);
			DATA_TYPE mva = MEM_R(addr);
			uint32_t  imm = instr_fetch(eip+1+len, imm_byte);
			
			concat(set_CF_, SUFFIX) (mva, imm, 1);
			concat(set_SF_, SUFFIX) (mva, imm, 1);
			concat(set_PF_, SUFFIX) (mva, imm, 1);
			concat(set_ZF_, SUFFIX) (mva, imm, 1);
			concat(set_OF_, SUFFIX) (mva, imm, 1);

			print_asm("cmp" str(SUFFIX) " 0x%x,%s", imm, ModR_M_asm);

			return 1 + len + imm_byte;
		}
		else
		{
			DATA_TYPE rva = REG(m.R_M);
			uint32_t  imm = instr_fetch(eip+1+1, imm_byte);

			concat(set_CF_, SUFFIX) (rva, imm, 1);
			concat(set_CF_, SUFFIX) (rva, imm, 1);
			concat(set_CF_, SUFFIX) (rva, imm, 1);
			concat(set_CF_, SUFFIX) (rva, imm, 1);
			concat(set_CF_, SUFFIX) (rva, imm, 1);

			print_asm("cmp" str(SUFFIX) " %x, %%%s", imm, REG_NAME(m.R_M));

			return 1 + 1 + imm_byte;
		} 
	}
	return 0;
}



#include "exec/template-end.h"

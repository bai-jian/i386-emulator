#include "exec/template-start.h"


#include "cpu/modrm.h"

make_helper( concat(cmp_i2r0_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip+1, DATA_BYTE);
	DATA_TYPE rva = REG(0);


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
			

			return 1 + len + imm_byte;
		}
		else
		{
			DATA_TYPE rva = REG(m.R_M);
			uint32_t  imm = instr_fetch(eip+1+1, imm_byte);


			print_asm("cmp" str(SUFFIX) " %x, %%%s", imm, REG_NAME(m.R_M));

			return 1 + 1 + imm_byte;
		} 
	}
	return 0;
}

make_helper( concat(cmp_r2rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t  addr;
		uint8_t   len = read_ModR_M(eip+1, &addr);
		DATA_TYPE rmv = MEM_R(addr);
		DATA_TYPE rva = REG(m.reg);


		return 1 + len;
	}
	else
	{ 
		DATA_TYPE rmv = REG(m.R_M);
		DATA_TYPE rva = REG(m.reg);


		return 1 + 1;
	}
}

make_helper( concat(cmp_rm2r_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{ 
		swaddr_t  addr;
		uint8_t   len = read_ModR_M(eip+1, &addr);
		DATA_TYPE rmv = MEM_R(addr);
		DATA_TYPE rva = REG(m.reg);


		return 1 + len;
	}
	else
	{
		DATA_TYPE rmv = REG(m.R_M);
		DATA_TYPE rva = REG(m.reg);


		return 1 + 1;
	}   
}


#include "exec/template-end.h"

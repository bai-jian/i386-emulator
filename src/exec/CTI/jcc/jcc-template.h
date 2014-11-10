#include "exec/helper.h"

#include "exec/template-start.h"

make_helper( concat(je_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip = ( cpu.ZF ? disp : 0 );
	print_asm("je    " "%x", eip + instr_len + disp);
	return instr_len;
}
	
make_helper( concat(jne_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ( !cpu.ZF ? disp : 0 );
	print_asm("jne   " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(ja_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((!cpu.CF && !cpu.ZF) ? disp : 0); 
	print_asm("ja    " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jb_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += (cpu.CF ? disp : 0);
	print_asm("jb    " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jae_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += (!cpu.CF ? disp : 0);
	print_asm("jae   " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jbe_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.CF || cpu.ZF) ? disp : 0);
	print_asm("jbe   " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jg_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((!cpu.ZF && cpu.SF == cpu.OF) ? disp : 0);
	print_asm("jg    " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jl_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.SF != cpu.OF) ? disp : 0);
	print_asm("jl    " "%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jge_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.SF == cpu.OF) ? disp : 0);
	print_asm("jge   " "%x", eip + instr_len + disp);
	return instr_len;	
}

make_helper( concat(jle_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.ZF || cpu.SF != cpu.OF) ? disp : 0);
	print_asm("jle   " "%x", eip + instr_len + disp);
	return instr_len;
}


#include "exec/template-end.h"

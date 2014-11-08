#include "exec/helper.h"

#include "exec/template-start.h"

make_helper( concat(je_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ( cpu.ZF ? disp : 0 );
	print_asm("je" " %x", eip + instr_len + disp);
	return instr_len;
}
	
make_helper( concat(jne_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jne" " %x", eip + 1 + DATA_BYTE + disp);

	return !cpu.ZF ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
}

make_helper( concat(ja_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("ja" " %x", eip + 1 + DATA_BYTE + disp);

	return (!cpu.CF && !cpu.ZF) ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
}

make_helper( concat(jb_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jb" " %x", eip + 1 + DATA_BYTE + disp);

	return cpu.CF ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
}

make_helper( concat(jae_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jae" " %x", eip + 1 + DATA_BYTE + disp);

	return !cpu.CF ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
}

make_helper( concat(jbe_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jbe" " %x", eip + 1 + DATA_BYTE + disp);

	return (cpu.CF || cpu.ZF) ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
}

make_helper( concat(jg_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jg" " %x", eip + 1 + DATA_BYTE + disp);

	return (!cpu.ZF && cpu.SF == cpu.OF) ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE); 
}

make_helper( concat(jle_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.ZF || cpu.SF != cpu.OF) ? disp : 0);
	print_asm("jle" " %x", eip + instr_len + disp);
	return instr_len;
}

#include "exec/template-end.h"

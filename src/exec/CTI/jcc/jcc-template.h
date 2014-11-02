#include "exec/helper.h"

#include "exec/template-start.h"

make_helper( concat(je_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("je" " %x", eip + 1 + DATA_BYTE + disp);

	return cpu.ZF ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
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

make_helper( concat(jle_, SUFFIX) )
{
	DATA_TYPE_S disp = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jle" " %x", eip + 1 + DATA_BYTE + disp);

	return (cpu.ZF || cpu.SF != cpu.OF) ? (1 + DATA_BYTE + disp) : (1 + DATA_BYTE);
}

#include "exec/template-end.h"

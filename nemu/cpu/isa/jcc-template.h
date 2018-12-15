#include "helper.h"
#include "template-start.h"

make_helper( concat(je_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ( cpu.ZF ? disp : 0 );
	print_asm("je     $0x%x", eip + instr_len + disp);
	return instr_len;
}
	
make_helper( concat(jne_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ( !cpu.ZF ? disp : 0 );
	print_asm("jne    $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(ja_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((!cpu.CF && !cpu.ZF) ? disp : 0); 
	print_asm("ja     $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jb_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += (cpu.CF ? disp : 0);
	print_asm("jb     $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jae_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += (!cpu.CF ? disp : 0);
	print_asm("jae    $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jbe_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.CF || cpu.ZF) ? disp : 0);
	print_asm("jbe    $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jg_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((!cpu.ZF && cpu.SF == cpu.OF) ? disp : 0);
	print_asm("jg     $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jl_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.SF != cpu.OF) ? disp : 0);
	print_asm("jl     $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(jge_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.SF == cpu.OF) ? disp : 0);
	print_asm("jge    $0x%x", eip + instr_len + disp);
	return instr_len;	
}

make_helper( concat(jle_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += ((cpu.ZF || cpu.SF != cpu.OF) ? disp : 0);
	print_asm("jle    $0x%x", eip + instr_len + disp);
	return instr_len;
}

make_helper( concat(js_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += (cpu.SF ? disp : 0);
	print_asm("js     $0x%x", disp);
	return instr_len;
}

make_helper( concat(jns_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;
	int32_t disp = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);
	cpu.eip += (!cpu.SF ? disp : 0);
	print_asm("jns    $0x%x", disp);
	return instr_len;
}


#include "template-end.h"

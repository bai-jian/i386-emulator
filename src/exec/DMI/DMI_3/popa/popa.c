#include "exec/helper.h"


uint32_t swaddr_read(swaddr_t addr, size_t len);
make_helper(popa_w)
{
	reg_w(R_DI) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;
	reg_w(R_SI) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;
	reg_w(R_BP) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;
	                                        cpu.esp += 2;
	reg_w(R_BX) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;
	reg_w(R_DX) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;
	reg_w(R_CX) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;
	reg_w(R_AX) = swaddr_read(cpu.esp, 2);  cpu.esp += 2;

	print_asm("popaw");

	return 1;
}
make_helper(popa_l)
{
	reg_l(R_EDI) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	reg_l(R_ESI) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	reg_l(R_EBP) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	                                         cpu.esp += 4;
	reg_l(R_EBX) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	reg_l(R_EDX) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	reg_l(R_ECX) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	reg_l(R_EAX) = swaddr_read(cpu.esp, 4);  cpu.esp += 4;

	print_asm("popal");

	return 1;
}

extern char suffix;
make_helper(popa_v)
{  return  suffix == 'l'  ?  popa_l(eip)  :  popa_w(eip);  }

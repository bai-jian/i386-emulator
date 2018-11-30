#include "exec/helper.h"


void swaddr_write(swaddr_t addr, size_t len, uint32_t data);
make_helper(pusha_w)
{
	uint16_t temp = reg_w(R_SP);
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_AX));
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_CX));
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_DX));
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_BX));
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, temp);
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_BP));
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_SI));
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, reg_w(R_DI));

	print_asm("pushaw");

	return 1;
}
make_helper(pusha_l)
{
	uint32_t temp = reg_l(R_ESP);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_EAX));
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_ECX));
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_EDX));
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_EBX));
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, temp);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_EBP));
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_ESI));
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, reg_l(R_EDI));

	print_asm("pushal");

	return 1;
}

extern char suffix;
make_helper(pusha_v)
{	return  suffix == 'l'  ?  pusha_l(eip)  :  pusha_w(eip);  }

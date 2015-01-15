#include "exec/helper.h"

#include "cpu/reg.h"
#include "io/port-io.h"


int out_i_b(swaddr_t eip);
int out_i_w(swaddr_t eip);
int out_i_l(swaddr_t eip);

int out_r_b(swaddr_t eip);
int out_r_w(swaddr_t eip);
int out_r_l(swaddr_t eip);


extern char suffix;
make_helper(out_i_v)
{	return suffix == 'l' ? out_i_l(eip) : out_i_w(eip);	}
make_helper(out_r_v)
{	return suffix == 'l' ? out_r_l(eip) : out_r_w(eip);	}


int out_i_b(swaddr_t eip)
{
	uint8_t imm = instr_fetch(eip + 1, 1);
	pio_write(imm, 1, reg_b(R_AL));
	print_asm("outb   $0x%x", imm);
	return 1 + 1;
}
int out_i_w(swaddr_t eip)
{
	uint8_t imm = instr_fetch(eip + 1, 1);
	pio_write(imm, 2, reg_w(R_AX));
	print_asm("outw   $0x%x", imm);
	return 1 + 2;
}
int out_i_l(swaddr_t eip)
{
	uint8_t imm = instr_fetch(eip + 1, 1);
	pio_write(imm, 4, reg_l(R_EAX));
	print_asm("outl   $0x%x", imm);
	return 1 + 4;
}

int out_r_b(swaddr_t eip)
{
	pio_write(reg_w(R_DX), 1, reg_b(R_AL));
	print_asm("outb   %%dx");
	return 1;
}
int out_r_w(swaddr_t eip)
{
	pio_write(reg_w(R_DX), 2, reg_w(R_AX));
	print_asm("outw   %%dx");
	return 1;
}
int out_r_l(swaddr_t eip)
{
	pio_write(reg_w(R_DX), 4, reg_l(R_EAX));
	print_asm("outl   %%dx");
	return 1;
}

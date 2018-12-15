#include "helper.h"
#include "cpu/reg.h"
#include "io/port-io.h"


int in_i_b(swaddr_t eip);
int in_i_w(swaddr_t eip);
int in_i_l(swaddr_t eip);

int in_r_b(swaddr_t eip);
int in_r_w(swaddr_t eip);
int in_r_l(swaddr_t eip);


extern char suffix;
make_helper(in_i_v)
{	return suffix == 'l' ? in_i_l(eip) : in_i_w(eip);	}
make_helper(in_r_v)
{	return suffix == 'l' ? in_r_l(eip) : in_r_w(eip);	}


int in_i_b(swaddr_t eip)
{
	uint8_t imm = instr_fetch(eip + 1, 1);
	reg_b(R_AL) = pio_read(imm, 1);
	print_asm("inb    $0x%x", imm);
	return 1 + 1;
}
int in_i_w(swaddr_t eip)
{
	uint8_t imm = instr_fetch(eip + 1, 1);
	reg_w(R_AX) = pio_read(imm, 2);
	print_asm("inw    $0x%x", imm);
	return 1 + 2;
}
int in_i_l(swaddr_t eip)
{
	uint8_t imm = instr_fetch(eip + 1, 1);
	reg_l(R_EAX) = pio_read(imm, 4);
	print_asm("inl    $0x%x", imm);
	return 1 + 4;
}

int in_r_b(swaddr_t eip)
{
	reg_b(R_AL) = pio_read(reg_w(R_DX), 1);
	print_asm("inb    %%dx");
	return 1;
}
int in_r_w(swaddr_t eip)
{
	reg_w(R_AX) = pio_read(reg_w(R_DX), 2);
	print_asm("inw    %%dx");
	return 1;
}
int in_r_l(swaddr_t eip)
{
	reg_l(R_EAX) = pio_read(reg_w(R_DX), 4);
	print_asm("inl    %%dx");
	return 1;
}

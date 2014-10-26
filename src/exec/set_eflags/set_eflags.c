#include "cpu/reg.h"

void set_ZF_b(uint8_t  valu)	{	cpu.ZF = valu == 0;	}
void set_ZF_w(uint16_t valu)	{	cpu.ZF = valu == 0;	}
void set_ZF_l(uint32_t valu)	{	cpu.ZF = valu == 0;	}

void set_SF_b(uint8_t  valu) 	{ 	cpu.SF = valu >> 7 ; }
void set_SF_w(uint16_t valu) 	{ 	cpu.SF = valu >> 15; }
void set_SF_l(uint32_t valu) 	{ 	cpu.SF = valu >> 31; }

void set_PF_b(uint8_t  valu)
{
	uint8_t num_1 = 0, valu_low8 = valu & 0xFF;
	while(valu_low8)
	{
		num_1 += valu_low8 % 2;
		valu_low8 = valu_low8 >> 1;
	}

	cpu.PF = num_1 % 2 == 0;
}

void set_PF_w(uint16_t valu)
{
	uint8_t num_1 = 0, valu_low8 = valu & 0xFF;
	while(valu_low8)
	{
		num_1 += valu_low8 % 2;
		valu_low8 = valu_low8 >> 1;
	}

	cpu.PF = num_1 % 2 == 0;
}

void set_PF_l(uint32_t valu)
{
	uint8_t num_1 = 0, valu_low8 = valu & 0xFF;
	while(valu_low8)
	{
		num_1 += valu_low8 % 2;
		valu_low8 = valu_low8 >> 1;
	}

	cpu.PF = num_1 % 2 == 0;
}

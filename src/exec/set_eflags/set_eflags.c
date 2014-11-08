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

void set_CF_b(uint8_t lopd,  uint8_t ropd,  uint8_t op)
{
	uint8_t add1 = lopd;
    uint8_t	add2 = op ? -ropd : ropd;
	uint8_t valu = add1 + add2;

	uint8_t add1_s = add1 >> 7;
	uint8_t add2_s = add2 >> 7;
	uint8_t valu_s = valu >> 7;

	cpu.CF = (add1_s && add2_s) || (add1_s && !valu_s) || (add2_s && !valu_s);
} 
void set_CF_w(uint16_t lopd, uint16_t ropd, uint16_t op)
{
	uint16_t add1 = lopd;
    uint16_t add2 = op ? -ropd : ropd;
	uint16_t valu = add1 + add2;

	uint16_t add1_s = add1 >> 15;
	uint16_t add2_s = add2 >> 15;
	uint16_t valu_s = valu >> 15;

	cpu.CF = (add1_s && add2_s) || (add1_s && !valu_s) || (add2_s && !valu_s);
}
void set_CF_l(uint32_t lopd, uint32_t ropd, uint32_t op)
{
	uint32_t add1 = lopd;
    uint32_t add2 = op ? -ropd : ropd;
	uint32_t valu = add1 + add2;

	uint32_t add1_s = add1 >> 31;
	uint32_t add2_s = add2 >> 31;
	uint32_t valu_s = valu >> 31;

	cpu.CF = (add1_s && add2_s) || (add1_s && !valu_s) || (add2_s && !valu_s);
} 

void set_OF_b(uint8_t lopd,  uint8_t ropd,  uint8_t op)
{
	
	uint8_t add1 = lopd;
    uint8_t add2 = op ? -ropd : ropd;
	uint8_t valu = add1 + add2;

	uint8_t add1_s = add1 >> 7;
	uint8_t add2_s = add2 >> 7;
	uint8_t valu_s = valu >> 7;

	cpu.OF = add1_s!=valu_s && add2_s!=valu_s;
}
void set_OF_w(uint16_t lopd, uint16_t ropd, uint16_t op)
{
	
	uint16_t add1 = lopd;
    uint16_t add2 = op ? -ropd : ropd;
	uint16_t valu = add1 + add2;

	uint16_t add1_s = add1 >> 15;
	uint16_t add2_s = add2 >> 15;
	uint16_t valu_s = valu >> 15;

	cpu.OF = add1_s!=valu_s && add2_s!=valu_s;
}
void set_OF_l(uint32_t lopd, uint32_t ropd, uint32_t op)
{
	
	uint32_t add1 = lopd;
    uint32_t add2 = op ? -ropd : ropd;
	uint32_t valu = add1 + add2;

	uint32_t add1_s = add1 >> 31;
	uint32_t add2_s = add2 >> 31;
	uint32_t valu_s = valu >> 31;

	cpu.OF = add1_s!=valu_s && add2_s!=valu_s;
}

#include "common.h"

#include "cpu/reg.h"


/* Memory Interfaces */
// uint32_t swaddr_read(swaddr_t addr, size_t len);
// void swaddr_write(swaddr_t addr, size_t len, uint32_t data);
// uint32_t instr_fetch(swaddr_t addr, size_t len);


lnaddr_t segment_translate(swaddr_t addr, size_t len, uint8_t cur_sreg);

uint32_t hwaddr_read(hwaddr_t addr, size_t len);
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data);

uint32_t lnaddr_read(lnaddr_t addr, size_t len);
void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data);

uint32_t swaddr_read(swaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);

	if (cpu.CR0_PE == 0)
		return hwaddr_read(addr, len);
	else
	{	
		if (cpu.CR0_PG == 0)
		{
			hwaddr_t hwaddr = segment_translate(addr, len, current_sreg);
			return hwaddr_read(hwaddr, len);
		}
		else
		{
			lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
			return lnaddr_read(lnaddr, len);
		}
	}
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);

	if (cpu.CR0_PE == 0)
		return hwaddr_write(addr, len, data);
	else
	{
		if (cpu.CR0_PG == 0)
		{
			hwaddr_t hwaddr = segment_translate(addr, len, current_sreg);
			return hwaddr_write(hwaddr, len, data);
		}
		else
		{
			lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
			return lnaddr_write(lnaddr, len, data);
		}
	}
}

uint32_t instr_fetch(swaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);

	if (cpu.CR0_PE == 0)
		return lnaddr_read(addr, len);
	else
	{
		if (cpu.CR0_PG == 0)
		{
			hwaddr_t hwaddr = segment_translate(addr, len, R_CS);
			return hwaddr_read(hwaddr, len);
		}
		else
		{
			lnaddr_t lnaddr = segment_translate(addr, len, R_CS);
			return lnaddr_read(lnaddr, len);
		}
	}
}

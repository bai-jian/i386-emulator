#include "common.h"

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);


/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{	return dram_read(addr, len);	}
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{	dram_write(addr, len, data);	}


uint32_t swaddr_read(swaddr_t addr, size_t len)
{	return hwaddr_read(addr, len);	}
void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{	hwaddr_write(addr, len, data);	}


static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len)
{	return dram_read(addr, len);	}
uint32_t instr_fetch(swaddr_t addr, size_t len)
{	return hwaddr_read_instr(addr, len);	}

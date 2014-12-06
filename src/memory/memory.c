#include "common.h"
/*
uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
*/

uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);


static uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{	return	cache_read(addr, len);	}
static void     hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{	        cache_write(addr, len, data);	}
static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len)
{	return	cache_read(addr, len);	}



/* Memory accessing interfaces */
uint32_t swaddr_read(swaddr_t addr, size_t len)
{	return		hwaddr_read(addr, len);	}
void     swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{	       		hwaddr_write(addr, len, data);	}
uint32_t instr_fetch(swaddr_t addr, size_t len)
{	return		hwaddr_read_instr(addr, len);	}

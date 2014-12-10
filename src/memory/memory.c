#include "common.h"


/* memory read/write interfaces */
uint32_t swaddr_read(swaddr_t addr, size_t len);
void swaddr_write(swaddr_t addr, size_t len, uint32_t data);
uint32_t instr_fetch(swaddr_t addr, size_t len);

static uint32_t hwaddr_read(hwaddr_t addr, size_t len);
static void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data);
static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len);



/* Function Definition */
/*
uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);
*/
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t addr, size_t, uint32_t);

static uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return	dram_read(addr, len);
}
static void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	dram_write(addr, len, data);
}
static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return	dram_read(addr, len);
}


uint32_t swaddr_read(swaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return  hwaddr_read(addr, len);
}
void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	hwaddr_write(addr, len, data);
}
uint32_t instr_fetch(swaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return  hwaddr_read_instr(addr, len);
}

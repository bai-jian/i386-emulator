#include "common.h"


uint32_t swaddr_read(swaddr_t addr, size_t len);
void swaddr_write(swaddr_t addr, size_t len, uint32_t data);
uint32_t instr_fetch(swaddr_t addr, size_t len);

/*
uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);

static uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return	cache_read(addr, len);
}
static void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	cache_write(addr, len, data);
}
static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return	cache_read(addr, len);
}
*/

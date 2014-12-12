#include "common.h"


uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);


uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return cache_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	return cache_write(addr, len, data);
}

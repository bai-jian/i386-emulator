#include "common.h"


uint32_t hwaddr_read(hwaddr_t addr, size_t len);
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data);


uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	return hwaddr_write(addr, len, data);
}

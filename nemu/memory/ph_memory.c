#include "common.h"
#include "cpu/io.h"

/* physical memory <- cache <- cache_L2 <- dram */
uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{	
	if (mmio_check(addr))
	{
		uint32_t data;
		mmio_read(addr, len, &data);
		return data;
	}
	return cache_read(addr, len);
}
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	if (mmio_check(addr))
		return mmio_write(addr, len, data);
	return cache_write(addr, len, data);
}


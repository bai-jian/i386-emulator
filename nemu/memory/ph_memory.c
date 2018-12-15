#include "common.h"
#include "cpu/io.h"

/* physical memory <- cache <- cache_L2 <- dram */
uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{	
	if (i386_mmio.check(addr))
	{
		uint32_t data;
		i386_mmio.read(addr, len, &data);
		return data;
	}
	return cache_read(addr, len);
}
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	if (i386_mmio.check(addr))
		return i386_mmio.write(addr, len, data);
	return cache_write(addr, len, data);
}


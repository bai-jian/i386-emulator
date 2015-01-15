#include "common.h"

#include "io/mmio.h"

/* physical memory <- cache <- cache_L2 <- dram */
uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);
/* physical memory <- I/O                       */

uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{	
	int mmio_num = is_mmio(addr);
	if (mmio_num == -1)
		return cache_read(addr, len);
	else
		return mmio_read(addr, len, mmio_num);
}
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	int mmio_num = is_mmio(addr);
	if (mmio_num == -1)
		return  cache_write(addr, len, data);
	else
		return mmio_write(addr, len, data, mmio_num);
}


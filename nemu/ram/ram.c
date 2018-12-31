#include "ram/ram.h"

void ram_read(phyaddr_t addr, size_t len, uint8_t *data)
{
	uint32_t temp_data = cache_read(addr, len);
	
	uint8_t temp[4];
	temp[0] = temp_data & 0xff;
	temp[1] = (temp_data >> 8) & 0xff;
	temp[2] = (temp_data >> 16) & 0xff;
	temp[3] = (temp_data >> 24) & 0xff;
	memcpy(data, temp, len);
}

void ram_write(phyaddr_t addr, size_t len, uint8_t *data)
{
	uint32_t temp = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	cache_write(addr, len, temp);
}


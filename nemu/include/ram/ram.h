#ifndef __RAM_H__
#define __RAM_H__

#include "common.h"

void ram_read(phyaddr_t addr, size_t len, uint8_t *data);
void ram_write(phyaddr_t addr, size_t len, uint8_t *data);

uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);

uint32_t cache_L2_read(hwaddr_t, size_t);
void cache_L2_write(hwaddr_t, size_t, uint32_t);

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

#endif

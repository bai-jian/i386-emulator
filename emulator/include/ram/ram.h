#ifndef __RAM_H__
#define __RAM_H__

#include "common.h"

#define PHY_MEM_SIZE (1 << 27)
uint8_t *phy_mem;

void test_ram();

void flush_cache();
void flush_l2cache();
void flush_dram();

void ram_read(phyaddr_t addr, size_t len, uint8_t *data);
void ram_write(phyaddr_t addr, size_t len, uint8_t *data);

void cache_read(phyaddr_t addr, size_t len, uint8_t *data);
void cache_write(phyaddr_t addr, size_t len, uint8_t *data);

void l2cache_read(phyaddr_t addr, size_t len, uint8_t *data);
void l2cache_write(phyaddr_t addr, size_t len, uint8_t *data);

void dram_read(phyaddr_t addr, size_t len, uint8_t *data);
void dram_write(phyaddr_t addr, size_t len, uint8_t *data);

#endif

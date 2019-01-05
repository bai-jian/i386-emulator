#include "ram/ram.h"

#define BIB_WIDTH 6  // BIB: Bytes In a Block
#define WAY_WIDTH 4
#define SET_WIDTH (22 - BIB_WIDTH - WAY_WIDTH)

#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)

struct block_t
{
	uint8_t valid : 1;
	uint8_t dirty : 1;
	uint32_t tag;
	uint8_t block[NR_BIB];
} block[NR_SET][NR_WAY];

#define AIB_WIDTH BIB_WIDTH  // AIB: Address In a Block
#define IDX_WIDTH SET_WIDTH
#define TAG_WIDTH (32 - AIB_WIDTH - IDX_WIDTH)

union l2cache_addr_t
{
	uint32_t addr;
	struct
	{ 
		uint32_t aib : AIB_WIDTH;
		uint32_t idx : IDX_WIDTH;
		uint32_t tag : TAG_WIDTH;
	};
};

void flush_l2cache()
{
	int i, j;
	for (i = 0; i < NR_SET; ++i)
		for (j = 0; j < NR_WAY; ++j)
			block[i][j].valid = 0;
}

static int l2cache_hit(union l2cache_addr_t l2cache_addr)
{
	int i;
	for (i = 0; i < NR_WAY; i++)
		if (block[l2cache_addr.idx][i].valid && block[l2cache_addr.idx][i].tag == l2cache_addr.tag)
			return i;
	return -1;
}

static int l2cache_replace(union l2cache_addr_t l2cache_addr)
{
	l2cache_addr.aib = 0;

	uint32_t way, i;
	for (way = 0; way < NR_WAY; way++)
		if (!block[l2cache_addr.idx][way].valid)
			break;
	if (way == NR_WAY)
	{
		way = (l2cache_addr.tag + l2cache_addr.idx + l2cache_addr.aib) % NR_WAY;
	}

	if (block[l2cache_addr.idx][way].valid && block[l2cache_addr.idx][way].dirty)
	{
		for (i = 0; i < NR_BIB; i += 4)
			dram_write(l2cache_addr.addr + i, 4, block[l2cache_addr.idx][way].block + i);
	}	

	block[l2cache_addr.idx][way].valid = 1;
	block[l2cache_addr.idx][way].dirty = 0;
	block[l2cache_addr.idx][way].tag = l2cache_addr.tag;

	for (i = 0; i < NR_BIB; i += 4)
		dram_read(l2cache_addr.addr + i, 4, block[l2cache_addr.idx][way].block + i);

	return way;
}

void l2cache_read(phyaddr_t addr, size_t len, uint8_t *data)
{
	union l2cache_addr_t l2cache_addr = (union l2cache_addr_t)addr;

	int way = l2cache_hit(l2cache_addr);
	if (way == -1)
		way = l2cache_replace(l2cache_addr);

	memcpy(data, block[l2cache_addr.idx][way].block + l2cache_addr.aib, len);
}

void l2cache_write(phyaddr_t addr, size_t len, uint8_t *data)
{
	union l2cache_addr_t l2cache_addr = (union l2cache_addr_t)addr;

	int way = l2cache_hit(l2cache_addr);
	if (way != -1)
	{
		memcpy(block[l2cache_addr.idx][way].block + l2cache_addr.aib, data, len);
		block[l2cache_addr.idx][way].dirty = 1;
	}
	else
	{
		dram_write(l2cache_addr.addr, len, data);
		l2cache_replace(l2cache_addr);
	}
}


#include "ram/ram.h"

#define BIB_WIDTH 6  // BIB: Bytes In a Block
#define WAY_WIDTH 3
#define SET_WIDTH (16 - BIB_WIDTH - WAY_WIDTH)

#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)

struct block_t
{
	uint8_t valid : 1;
	uint32_t tag;
	uint8_t block[NR_BIB];
} block[NR_SET][NR_WAY];

#define AIB_WIDTH BIB_WIDTH  //AIB: Address In a Block
#define IDX_WIDTH SET_WIDTH
#define TAG_WIDTH (32 - AIB_WIDTH - IDX_WIDTH)

union cache_addr_t
{
	phyaddr_t addr;
	struct
	{
		phyaddr_t aib : AIB_WIDTH;
		phyaddr_t idx : IDX_WIDTH;
		phyaddr_t tag : TAG_WIDTH;
	};
};

void flush_cache()
{
	int i, j;
	for (i = 0; i < NR_SET; ++i)
		for (j = 0; j < NR_WAY; ++j)
			block[i][j].valid = 0;
}

static int cache_hit(union cache_addr_t cache_addr)
{
	int i;
	for (i = 0; i < NR_WAY; i++)
		if (block[cache_addr.idx][i].valid && block[cache_addr.idx][i].tag == cache_addr.tag)
			return i;
	return -1;
}

static int cache_replace(union cache_addr_t cache_addr)
{
	cache_addr.aib = 0;

	uint32_t way, i;
	for (way = 0; way < NR_WAY; way++)
		if (!block[cache_addr.idx][way].valid)
			break;
	if (way == NR_WAY)
		way = (cache_addr.tag + cache_addr.idx + cache_addr.aib) % NR_WAY;

	block[cache_addr.idx][way].valid = 1;
	block[cache_addr.idx][way].tag = cache_addr.tag;

	for (i = 0; i < NR_BIB; i += 4)
		l2cache_read(cache_addr.addr + i, 4, block[cache_addr.idx][way].block + i);

	return way;
}

void cache_read(phyaddr_t addr, size_t len, uint8_t *data)
{
	union cache_addr_t cache_addr = (union cache_addr_t)addr;

	int way = cache_hit(cache_addr);
	if (way == -1)
		way = cache_replace(cache_addr);

	memcpy(data, block[cache_addr.idx][way].block + cache_addr.aib, len);
}

void cache_write(phyaddr_t addr, size_t len, uint8_t *data)
{
	union cache_addr_t cache_addr = (union cache_addr_t)addr;

	int way = cache_hit(cache_addr);
	if (way != -1)
	{
		memcpy(block[cache_addr.idx][way].block + cache_addr.aib, data, len);
		l2cache_write(cache_addr.addr, len, data);
	}
	else
	{
		l2cache_write(cache_addr.addr, len, data);
	}
}


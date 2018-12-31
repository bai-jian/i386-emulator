#include "ram/ram.h"

/* Cache L2 Memory Size: 4MB = (4096 Set) * (16 Way/Set) * (64 B/Block) */
#define BIB_WIDTH 6  //BIB: Bytes In a Block
#define WAY_WIDTH 4
#define SET_WIDTH 12


/* Define [Cache L2] & [Block in Cache L2] */
#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)
struct BLOCK_L2
{
	struct
	{
		uint8_t valid	: 1;
		uint8_t dirty	: 1;
	};
	uint32_t tag;
	uint8_t bib[NR_BIB];
} block_L2[NR_SET][NR_WAY];


/* the Mapping between [PA] and [CA] */
/*     PA:  Physical Address         */
/*     CA:  Cache L2 Address         */

#define AIB_WIDTH BIB_WIDTH  //AIB: Address In a Block
#define IDX_WIDTH SET_WIDTH
#define TAG_WIDTH (32 - AIB_WIDTH - IDX_WIDTH)

#define PA2CA_AIB(addr)  ((uint32_t)(addr)) << (IDX_WIDTH + TAG_WIDTH) >> (IDX_WIDTH + TAG_WIDTH)
#define PA2CA_IDX(addr)  ((uint32_t)(addr)) << TAG_WIDTH >> (TAG_WIDTH + AIB_WIDTH)
#define PA2CA_TAG(addr)  ((uint32_t)(addr)) >> (AIB_WIDTH + IDX_WIDTH)
typedef union
{
	uint32_t addr;
	struct
	{ 
		uint32_t aib	: AIB_WIDTH;
		uint32_t idx	: IDX_WIDTH;
		uint32_t tag	: TAG_WIDTH;
	};
} cache_L2_addr;

#define CA2PA_BLK(tag, idx)  ((tag) << (AIB_WIDTH+IDX_WIDTH)) + ((idx) << AIB_WIDTH)

#define BLK_ADDR(addr) ((addr) >> AIB_WIDTH << AIB_WIDTH)


/* the time counter of Cache_L2 hit and miss*/
uint32_t cache_L2_hit = 0, cache_L2_miss = 0;


/* Functions:
 *     init:  void init_cache_L2( );
 *     read:  uint32_t cache_L2_read(hwaddr_t addr, size_t len);
 *     write: void cache_L2_write(hwaddr_t addr, size_t len, uint32_t data);
 */
void init_cache_L2( )
{
	int i, j;
	for (i = 0; i < NR_SET; ++i)
		for (j = 0; j < NR_WAY; ++j)
			block_L2[i][j].valid = 0;

	cache_L2_hit = 0;  cache_L2_miss = 0;
}


uint32_t cache_L2_read(hwaddr_t addr, size_t len)
{
	cache_L2_addr fstb;  fstb.addr = addr;            //fstb: the first byte
	cache_L2_addr lstb;  lstb.addr = addr + len - 1;  //lstb: the last byte

	// Judge whether data crosses the boundary
	if (fstb.idx != lstb.idx)  // data crossing
 	{
		hwaddr_t addr2 = BLK_ADDR(lstb.addr);
		size_t len1 = addr2 - addr;
		size_t len2 = len - len1;
		uint32_t data1 = cache_L2_read(addr, len1);
		uint32_t data2 = cache_L2_read(addr2, len2);
		return (data2 << (8*len1)) + data1;  // little endian
	}
	else                       // data not crossing
 	{
		// Judge hit or miss
		int way;
		for (way = 0; way < NR_WAY; ++way)
			if ( block_L2[fstb.idx][way].valid && (block_L2[fstb.idx][way].tag == fstb.tag) )
				break;

		// hit or miss calculates
		(way < NR_WAY)  ?  ++cache_L2_hit  :  ++cache_L2_miss;

		if (way == NR_WAY)  // miss
		{
			// Judge load or replace
			// Replacement Algorithm: randomized algorithm, replace BLOCK 0
			int i;
			for (i = 0; i < NR_WAY; ++i)
				if ( !block_L2[fstb.idx][i].valid )
					break;
			way = (i < NR_WAY) ? i : 0;

			block_L2[fstb.idx][way].valid = 1;
			block_L2[fstb.idx][way].dirty = 0;
			block_L2[fstb.idx][way].tag = fstb.tag;

			hwaddr_t blk_addr = BLK_ADDR(fstb.addr);
			for (i = 0; i < NR_BIB; ++i)
				block_L2[fstb.idx][way].bib[i] = dram_read(blk_addr+i, 1);
		}
		
		return  *(uint32_t*)(block_L2[fstb.idx][way].bib + fstb.aib)  &  (0xFFFFFFFFu >> ((4-len)<<3));
	}
}


// Write Back and Write Allocate
void cache_L2_write(hwaddr_t addr, size_t len, uint32_t data)
{
	cache_L2_addr fstb;  fstb.addr = addr;            //fstb: the first byte
	cache_L2_addr lstb;  lstb.addr = addr + len - 1;  //lstb: the last byte

	// Judge whether data crosses the boundary
	if (fstb.idx != lstb.idx)  // data crossing
 	{
		hwaddr_t addr2 = BLK_ADDR(lstb.addr);
		size_t len1 = addr2 - addr;
		size_t len2 = len - len1;
		uint32_t data1 = data << (8*len2) >> (8*len2);
		uint32_t data2 = data >> (8*len1);
		cache_L2_write(addr, len1, data1);
		cache_L2_write(addr2, len2, data2);
		return;
	}
	else                       // data not crossing
 	{
		// Judge hit or miss
		int way;
		for (way = 0; way < NR_WAY; ++way)
			if ( block_L2[fstb.idx][way].valid && (block_L2[fstb.idx][way].tag == fstb.tag) )
				break;

		// hit or miss calculates
		(way < NR_WAY)  ?  ++cache_L2_hit  :  ++cache_L2_miss;

		if (way < NR_WAY)  // hit, write back
		{
			block_L2[fstb.idx][way].dirty = 1;  // write back
			int i;
			for (i = 0; i < len; ++i)
				block_L2[fstb.idx][way].bib[fstb.aib + i] = (uint8_t) ( (data >> (8*i)) & 0x000000FF );
		}
		else            // miss, write allocate(load or replacement)
		{
			dram_write(addr, len, data);

			// load or replacement
			// Replacement Algorithm: randomized algorithm, replace BLOCK 0
			int i;
			for (i = 0; i < NR_WAY; ++i)
				if ( !block_L2[fstb.idx][i].valid )
					break;
			way = (i < NR_WAY) ? i : 0;

			// Write Back
			if ( block_L2[fstb.idx][way].dirty )
			{
				hwaddr_t blk_addr = CA2PA_BLK(block_L2[fstb.idx][way].tag, fstb.idx);
				for (i = 0; i < NR_WAY; ++i)
				{
					uint8_t data_temp = block_L2[fstb.idx][way].bib[i];
					dram_write(blk_addr + i, 1, data_temp);
				}
			}

			// Write Allocate
			block_L2[fstb.idx][way].valid = 1;
			block_L2[fstb.idx][way].dirty = 0;
			block_L2[fstb.idx][way].tag = fstb.tag;

			hwaddr_t blk_addr = BLK_ADDR(fstb.addr);
			for (i = 0; i < NR_BIB; ++i)
				block_L2[fstb.idx][way].bib[i] = dram_read(blk_addr + i, 1);
		}
	}
}


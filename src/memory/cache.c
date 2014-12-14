#include "common.h"


/* physical memory interfaces <- cache <- cache L2 */
uint32_t cache_L2_read(hwaddr_t, size_t);
void cache_L2_write(hwaddr_t, size_t, uint32_t);

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{	return  cache_read(addr, len);  }
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{	return  cache_write(addr, len, data);  }


/* Cache Memory Size: 64KB = (128 Set) * (8 Way/Set) * (64 B/Block) */
#define BIB_WIDTH 6  //BIB: Bytes In a Block
#define WAY_WIDTH 3
#define SET_WIDTH 7


/* Define [Cache] & [BLOCK] */
#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)
struct BLOCK
{
	bool valid;
	uint32_t tag;
	uint8_t bib[NR_BIB];
} block[NR_SET][NR_WAY];


/* the Mapping between [PA] and [CA] */
/*     PA:  Physical Address         */
/*     CA:  Cache Address            */

#define AIB_WIDTH  BIB_WIDTH  //AIB: Address In a Block
#define IDX_WIDTH  SET_WIDTH
#define TAG_WIDTH  (32 - AIB_WIDTH - IDX_WIDTH)
typedef union
{
	uint32_t addr;
	struct
	{
		uint32_t aib	: AIB_WIDTH;
		uint32_t idx	: IDX_WIDTH;
		uint32_t tag	: TAG_WIDTH;
	};
} cache_addr;

#define CA2PA_BLK(tag, idx)  ((tag) << (AIB_WIDTH+IDX_WIDTH)) + ((idx) << AIB_WIDTH)
#define BLK_ADDR(addr)  ((addr) >> AIB_WIDTH << AIB_WIDTH)


/* the time counter of Cache hit and miss */
uint32_t hit = 0, miss = 0;


/* Functions:
 *     init:  void init_cache( );
 *     read:  uint32_t cache_read(hwaddr_t addr, size_t len);
 *     write: void cache_write(hwaddr_t addr, size_t len, uint32_t data);
 */
void init_cache( )
{
	int i, j;
	for (i = 0; i < NR_SET; ++i)
		for (j = 0; j < NR_WAY; ++j)
			block[i][j].valid = 0;

	hit = 0;  miss = 0;
}


uint32_t cache_read(hwaddr_t addr, size_t len)
{
	cache_addr fstb;  fstb.addr = addr;            //fstb: the first byte
	cache_addr lstb;  lstb.addr = addr + len - 1;  //lstb: the last byte
	
	// Judge whether data crosses the boundary
	if (fstb.idx != lstb.idx)  // data crossing
	{
		hwaddr_t addr2 = BLK_ADDR(lstb.addr);
		size_t len1 = addr2 - addr;
		size_t len2 = len - len1;
		uint32_t data1 = cache_read(addr, len1);
		uint32_t data2 = cache_read(addr2, len2);
		return (data2 << (8*len1)) + data1;  // little endian
	}
	else                       // data not crossing
	{
		// Judge hit or miss
		int way;
		for (way = 0; way < NR_WAY; ++way)
			if ( block[fstb.idx][way].valid && (block[fstb.idx][way].tag == fstb.tag) )
				break;

		// hit or miss calculates
		(way < NR_WAY)  ?  ++hit  :  ++miss;

		if (way == NR_WAY)  //miss
		{
			// Judge load or replace
			// Replacement Algorithm: randomized algorithm, replace BLOCK 0
			int i;
			for (i = 0; i < NR_WAY; ++i)
				if ( !block[fstb.idx][i].valid )
					break;
			way = (i < NR_WAY) ? i : 0;

			block[fstb.idx][way].valid = 1;
			block[fstb.idx][way].tag = fstb.tag;

			hwaddr_t blk_addr = BLK_ADDR(fstb.addr);
			for (i = 0; i < NR_BIB; ++i)
				block[fstb.idx][way].bib[i] = cache_L2_read(blk_addr+i, 1);
		}

		return *(uint32_t*)(block[fstb.idx][way].bib + fstb.aib) & (0xFFFFFFFFu >> ((4-len)<<3));
	}
}


void cache_write(hwaddr_t addr, size_t len, uint32_t data)
{
	cache_addr fstb;  fstb.addr = addr;            //fstb: the first byte
	cache_addr lstb;  lstb.addr = addr + len - 1;  //lstb: the last byte

	// Judge whether data crosses the boundary
	if (fstb.idx != lstb.idx)  // data crossing
	{
		hwaddr_t addr2 = BLK_ADDR(lstb.addr);
		size_t len1 = addr2 - addr;
		size_t len2 = len - len1;
		uint32_t data1 = data << (8*len2) >> (8*len2);
		uint32_t data2 = data >> (8*len1);
		cache_write(addr, len1, data1);
		cache_write(addr2, len2, data2);
		return;
	}
	else  // data not crossing
	{
		// Judge hit or miss
		int way;
		for (way = 0; way < NR_WAY; ++way)
			if ( block[fstb.idx][way].valid && (block[fstb.idx][way].tag == fstb.tag) )
				break;

		// hit or miss calculates
		(way < NR_WAY)  ?  ++hit  :  ++miss;

		if (way < NR_WAY)  // hit, write through
		{
			// write cache itself
			int i;
			for (i = 0; i < len; ++i)
				block[fstb.idx][way].bib[fstb.aib + i] = (uint8_t) ( (data >> (8*i)) & 0x000000FF );
			// write cache L2
			cache_L2_write(addr, len, data);
		}
		else  // miss, write not allocate
		{	cache_L2_write(addr, len, data);  }

		return;
	}
}

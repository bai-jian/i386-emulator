#include "common.h"


uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);


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
		size_t len2 = lstb.addr - BLK_ADDR(lstb.addr) + 1;
		size_t len1 = len - len2;
		uint32_t data2 = cache_L2_read(lstb.addr, len2);
		uint32_t data1 = cache_L2_read(fstb.addr, len1);
		return (data2 << (8*len1)) + data1;  // little endian
	}
	else                       // data not crossing
 	{
		// Judge hit or miss
		int i;
		for (i = 0; i < NR_WAY; ++i)
			if ( block_L2[fstb.idx][i].valid && (block_L2[fstb.idx][i].tag == fstb.tag) )
				break;

		// hit or miss calculates
		(i < NR_WAY)  ?  ++cache_L2_hit  :  ++cache_L2_miss;

		// miss  and  (load or replacement)
		// Replacement Algorithm: randomized algorithm, replace BLOCK 0
		if (i == NR_WAY)
		{
			uint32_t way_num;

			int j;
			for (j = 0; j < NR_WAY; ++j)
				if ( !block_L2[fstb.idx][j].valid )
					break;
			way_num = (j < NR_WAY) ? j : 0;

			block_L2[fstb.idx][way_num].valid = 1;
			block_L2[fstb.idx][way_num].dirty = 0;
			block_L2[fstb.idx][way_num].tag = fstb.tag;

			hwaddr_t blk_addr = BLK_ADDR(fstb.addr);
			for (j = 0; j < NR_BIB; ++j)
				block_L2[fstb.idx][way_num].bib[j] = dram_read(blk_addr+j, 1);
		}
		
		return  *(uint32_t*)(block_L2[fstb.idx][i].bib + fstb.aib)  &  (~0u >> ((4-len)<<3));
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
		size_t len2 = lstb.addr - BLK_ADDR(lstb.addr) + 1;
		size_t len1 = len - len2;
		uint32_t data1 = data << (8*len2) >> (8*len2);
		uint32_t data2 = data >> (8*len1);
		cache_L2_write(fstb.addr, len1, data1);
		cache_L2_write(lstb.addr, len2, data2);
		return;
	}
	else                       // data not crossing
 	{
		// Judge hit or miss
		int i;
		for (i = 0; i < NR_WAY; ++i)
			if ( block_L2[fstb.idx][i].valid && (block_L2[fstb.idx][i].tag == fstb.tag) )
				break;

		// hit or miss calculates
		(i < NR_WAY)  ?  ++cache_L2_hit  :  ++cache_L2_miss;

		if (i < NR_WAY)  // hit, write back
		{
			block_L2[fstb.idx][i].dirty = 1;  // write back
			int j;
			for (j = 0; j < len; ++j)
				block_L2[fstb.idx][i].bib[fstb.aib + j] = (uint8_t) ( (data >> (8*j)) & 0x000000FF );
		}
		else            // miss, write allocate(load or replacement)
		{
			dram_write(addr, len, data);

			// load or replacement
			// Replacement Algorithm: randomized algorithm, replace BLOCK 0
			uint32_t way_num;
			int j;
			for (j = 0; j < NR_WAY; ++j)
				if ( !block_L2[fstb.idx][j].valid )
					break;
			way_num = (j < NR_WAY) ? j : 0;

			// Write Back
			if ( block_L2[fstb.idx][way_num].dirty )
			{
				hwaddr_t blk_addr = CA2PA_BLK(block_L2[fstb.idx][way_num].tag, fstb.idx);
				for (j = 0; j < NR_WAY; ++j)
				{
					uint8_t data_temp = block_L2[fstb.idx][way_num].bib[j];
					dram_write(blk_addr + j, 1, data_temp);
				}
			}

			// Write Allocate
			block_L2[fstb.idx][way_num].valid = 1;
			block_L2[fstb.idx][way_num].dirty = 0;
			block_L2[fstb.idx][way_num].tag = fstb.tag;

			hwaddr_t blk_addr = BLK_ADDR(fstb.addr);
			for (j = 0; j < NR_BIB; ++j)
				block_L2[fstb.idx][way_num].bib[i] = dram_read(blk_addr + j, 1);
		}
	}
}


/*

		}
		// miss  and  (load or replacement)
		// Replacement Algorithm: randomized algorithm, replace BLOCK 0
		if (i == NR_WAY)
		{
			uint32_t way_num;

			int j;
			for (j = 0; j < NR_WAY; ++j)
				if ( !block_L2[fstb.idx][j].valid )
					break;
			way_num = (j < NR_WAY) ? j : 0;

			block_L2[fstb.idx][way_num].valid = 1;
			block_L2[fstb.idx][way_num].dirty = 0;
			block_L2[fstb.idx][way_num].tag = fstb.tag;

			uint32_t blk_addr = BLK_ADDR(addr);
			for (j = 0; j < NR_BIB; ++j)
				block_L[fstb.idx][way_num].bib[j] = dram_read(blk_addr+j, 1);
		}
	}
}
{
	cache_L2_addr temp;
	temp.addr = addr;
*/
	/* Judge whether the first byte is in the cache */
/*	int j;
	for (j = 0; j < NR_WAY; ++j)
		if ( block[index][j].valid && (block[index][j].tag == tag) )
			break;

	if (j < NR_WAY) //hit first
	{
		++cache_L2_hit;
*/
		/* data cross the boundray */
/*		cache_L2_addr temp2;
		temp2.addr = addr + len - 1;

		if (temp2.index == temp.index)
		{
			block[temp.index][j].dirty = 1;
			switch(len)
			{
				case 1: *(uint8_t*)(block[temp.index][j].bib + temp.byte) = (uint8_t)data;
				case 2: *(uint16_t*)(block[temp.index][j].bib + temp.byte) = (uint16_t)data;
				case 4: *(uint32_t*)(block[temp.index][j].bib + temp.byte) = (uint32_t)data;
			}
		}
		else
		{
			int j2;
			for (j2 = 0; j2 < NR_WAY; ++j2)
				if ( block[temp2.index][j2].valid  &&  (block[temp2.index][j2].tag == temp2.tag) )
					break;

			if (j2 < NR_WAY) //hit again
			{
				block[temp.index][j].dirty = 1;
				block[temp2.index][j2].dirty = 1;

				size_t len2 = temp2.addr - (temp2.addr>>BIB_WIDTH<<BIB_WIDTH) + 1;
				size_t len1 = len - len2;
				int i;
				for (i = 0; i < len1; ++i)
					block[temp.index][j].bib[temp.byte + i] = GetByte(data, i);
				for (i = 0; i < len2; ++i)
					block[temp2.index][j2].bib[i] = GetByte(data, len1+i);
			}
			else			//miss
			{
				block[temp.index][j].dirty = 1;

				size_t len2 = temp2.addr - (temp2.addr>>BIB_WIDTH<<BIB_WIDTH) + 1;
				size_t len1 = len - len2;
				int i;
				for (i = 0; i < len1; ++i)
					block[temp.index][j].bib[temp.byte + i] = GetByte(data, i);

				//Replacement Algorithm: randomized algorithm, replace BLOCK 0

				uint32_t way_num;

				for (i = 0; i < NR_WAY; ++i)
					if ( !block[temp2.index][j2].valid )
						break;
				way_num = (i < NR_WAY) ? i : 0;
				
				if ( block[index2][way_num].dirty )
				{
					swaddr_t addr_temp = BlockAddr_to_hwaddr(block[index2][way_num].tag, index2);
					for (i = 0; i < NR_WAY; ++i)
						swaddr_write(addr_temp+i, 1, block[index2][way_num].bib[i]);
				}

				block[index2][way_num].valid = 1;
				block[index2][way_num].dirty = 0;
				block[index2][way_num].tag = temp2.tag;

				uint32_t addr_temp = hwaddr_to_BlockAddr(temp2.addr);
				for (i = 0; i < NR_BIB; ++i)
					Block[index2][way_num].bib[i] = dram_read(addr_temp+i, 1);

				return dram_read(addr, len);
			}
		}
	}
	else			//miss
	{
	}
		dram_write(addr, len, data);

		//Replacement Algorithm: randomized algorithm, replace BLOCK 0
		
		uint32_t way_num;

		int i
		for (i = 0; i < NR_WAY; ++i)
			if ( !block[temp.index][j].valid )
				break;
		way_num = (i < NR_WAY) ? i : 0;

		//Write Back
		if ( block[temp.index][j].dirty )
		{
			hwaddr_t addr_temp = BlockAddr_to_hwaddr(block[temp.index][way_num].tag, index);
			for (i = 0; i < NR_WAY; ++i)
				swaddr_write(addr_temp+i, 1, block[temp.index][way_num].bib[i]);
		}

		//Write Allocate
		block[temp.index][way_num].valid = 1;
		block[temp.index][way_num].dirty = 0;
		block[temp.index][way_num].tag = temp.tag;

		uint32_t addr_temp = hwaddr_to_BlockAddr(temp.addr);
		for (i = 0; i < NR_BIB; ++i)
			block[temp.index][way_num].bib[i] = dram_read(addr_temp+i, 1);
}*/

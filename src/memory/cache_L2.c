
#include "common.h"
/*
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
*/
/* Cache L2 Memory Size: 4MB = (4096 Set) * (16 Way/Set) * (64 B/Block) */
#define BIB_WIDTH 6  //BIB: Bytes In a Block
#define WAY_WIDTH 4
#define SET_WIDTH 12

#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)


/* Define [Cache L2] & [Block in Cache L2] */
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

/* the Mapping between [VA] and [CA] */
/*     VA:  Virtual Address          */
/*     CA:  Cache L2 Address         */
//#define 
typedef union
{
	uint32_t addr;
	struct
	{ 
		uint32_t byte	: BIB_WIDTH;
		uint32_t index	: SET_WIDTH;
		uint32_t tag	: (32 - SET_WIDTH - BIB_WIDTH);
	};
} cache_L2_addr;

/*
uint32_t cache_L2_hit = 0, cache_L2_miss = 0;

void init_cache_L2( )
{
	int i, j;
	for (i = 0; i < NR_SET; ++i)
		for (j = 0; j < NR_WAY; ++j)
			block[i][j].valid = 0;

	cache_L2_hit = 0;  cache_L2_miss = 0;
}

uint32_t cache_L2_read(hwaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);

	cache_L2_addr temp;
	temp.addr = addr;
*/
	/* Judge whether the first byte is in the cache */
/*	int j;
	for (j = 0; j < NR_WAY; ++j)
		if ( !block[temp.index][j].dirty  &&  block[temp.index][j].valid  &&  \
			 (block[temp.index][j].tag == temp.tag) )
			break;

	if (j < NR_WAY) //hit first
	{
		++cache_L2_hit;
*/
		/* data cross the boundary */
/*		cache_addr temp2;
		temp2.addr = addr + len -1;
		
		if (temp2.index == temp.index)
			return *(uint32_t*)(block[temp.index][j].bib + temp.byte)  &  (~0u >> ((4-len)<<3));
		else
		{
			int j2;
			for (j2 = 0; j2 < NR_WAY; ++j2)
				if ( !block[temp2.index][j2].dirty  &&  block[temp2.index][j2].valid  &&  (block[temp2.index][j2].tag == temp2.tag) )
					break;

			if (j2 < NR_WAY) //hit again
			{
				uint8_t data[2 * NR_BIB];

				int k;
				for (k = 0; k < NR_BIB; ++k)
				{
					data[k] = block[temp.index][j].bib[k];
					data[NR_BIB+k] = block[temp2.index][j2].bib[k];
				}

				return *(uint32_t*)(data + temp.byte)  &  (~0u >> ((4-len)<<3));
			}
			else
			{
				//Replacement Algorithm: randomized algorithm, replace BLOCK 0
				
				uint32_t way_num;

				int i;
				for (i = 0; i < NR_WAY; ++i)
					if ( block[temp2.index][i].dirty  ||  !block[temp2.index][i].valid )
						break;
				way_num = (i < NR_WAY) ? i : 0;

				block[temp2.index][way_num].valid = 1;
				block[temp2.index][way_num].dirty = 0;
				block[temp2.index][way_num].tag = temp2.tag;

				uint32_t addr_temp = temp2.addr >> BIB_WIDTH << BIB_WIDTH;
				for (i = 0; i < NR_BIB; ++i)
					block[temp2.index][way_num].bib[i] = dram_read(addr_temp+i, 1);

				return dram_read(addr, len);
			}
		}
	}
	else			//miss
	{
		++cache_L2_miss;

		//Replacement Algorithm: randomized algorithm, replace BLOCK 0

		uint32_t way_num;

		int i;
		for (i = 0; i < NR_WAY; ++i)
			if ( block[temp.index][i].dirty || !block[temp.index][i].valid )
				break;
		way_num = (i < NR_WAY) ? i : 0;

		block[temp.index][way_num].valid = 1;
		block[temp.index][way_num].dirty = 0;
		block[temp.index][way_num].tag = temp.tag;

		uint32_t addr_temp = addr >> BIB_WIDTH << BIB_WIDTH;
		for (i = 0; i < NR_BIB; ++i)
			block[temp.index][way_num].bib[i] = dram_read(addr_temp+i, 1);

		return dram_read(addr, len);
	}
}
*/

#define GetByte(data, i)  ( (data) & ( 0xFF << (8*(i)) ) )   >>   (8*(i))

#define BlockAddr_to_hwaddr(tag, index)  \
	((tag) << (BIB_WIDTH + SET_WIDTH)) + ((index) << BIB_WIDTH)
#define hwaddr_to_BlockAddr(addr)  \
	(addr) >> BIB_WIDTH << BIB_WIDTH


/* Write Back and Write Allocate *//*
void cache_L2_write(hwaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);

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
	}
}












*/


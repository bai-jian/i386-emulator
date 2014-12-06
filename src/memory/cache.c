#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);


/* Cache Memory Size: 64KB = (128 Set) * (8 Way/Set) * (64 B/BLOCK) */
#define BIB_WIDTH 6  //BPB: Bytes In a Block
#define WAY_WIDTH 3
#define SET_WIDTH 7

#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)

/* Define [Cache] & [BLOCK] */
struct BLOCK
{
	bool valid;
	uint32_t tag;
	uint8_t bib[NR_BIB];
} block[NR_SET][NR_WAY];

/* Define [Cache Address] */
typedef union
{
	uint32_t addr;
	struct
	{
		uint32_t byte	: BIB_WIDTH;
		uint32_t index	: SET_WIDTH;
		uint32_t tag	: (32 - SET_WIDTH - BIB_WIDTH);
	};
} cache_addr;

void init_cache( )
{
	int i, j;
	for (i = 0; i < NR_SET; ++i)
		for (j = 0; j < NR_WAY; ++j)
			block[i][j].valid = false;
}

static int count[2] = {0, 0};
uint32_t cache_read(hwaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);

	uint8_t data[2 * NR_BIB];

	cache_addr temp;
	temp.addr = addr;
	uint32_t tag = temp.tag;
	uint32_t index = temp.index;
	uint32_t byte = temp.byte;

	/* Judge whether the first byte is in the cache */
	int j;
	for (j = 0; j < NR_WAY; ++j)
		if ( block[index][j].valid && (block[index][j].tag == tag) )
			break;

	printf("hit = %d, miss = %d\n", count[0], count[1]);
	if (j < NR_WAY) //hit first
	{
		++(count[0]);
		int k;
		for (k = 0; k < NR_BIB; ++k)
			data[k] = block[index][j].bib[k];

		/* data cross the boundary */
		cache_addr temp2;
		temp2.addr = addr + len - 1;
		uint32_t tag2 = temp2.tag;
		uint32_t index2 = temp2.index;
		
		if ( index2 == index)
			return *(uint32_t*)(block[index][j].bib+byte) & (~0u>>((4-len)<<3));
		else
		{
			for (j = 0; j < NR_WAY; ++j)
				if ( block[index2][j].valid && (block[index2][j].tag == tag2) )
					break;

			if (j < NR_WAY)  //hit again
			{
				for (k = 0; k < NR_BIB; ++k)
					data[NR_BIB+k] = block[index][j].bib[k];

				return *(uint32_t*)(data+byte) & (~0u >> ((4-len)<<3));
			}
			else			//miss again
			{
				//Replacement Algorithm: randomized algorithm, replace BLOCK 0
				block[index][0].valid = true;
				block[index][0].tag = tag2;

				temp2.byte = 0;
				int i;
				for (i = 0; i < NR_BIB; ++i)
					block[index][0].bib[i] = dram_read(temp2.addr+i, 1);

				return dram_read(addr, len);
			}
		}
	} 
	else			//miss
	{
		++count[1];
		//Replacement Algorithm: randomized algorithm, replace BLOCK 0
		block[index][0].valid = true;
		block[index][0].tag = tag;

		temp.byte = 0;
		int i;
		for (i = 0; i < NR_BIB; ++i)
			block[index][0].bib[i] = dram_read(temp.addr+i, 1);

		return dram_read(addr, len);
	}
}

/* Write Through and Not Write Allocate */
void cache_write(hwaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);

	cache_addr temp;
	temp.addr = addr;
	uint32_t tag = temp.tag;
	uint32_t index = temp.index;
	uint32_t byte = temp.byte;

	/* Judge whether the first byte is in the cache */
	int j;
	for (j = 0; j < NR_WAY; ++j)
		if ( block[index][j].valid && (block[index][j].tag == tag) )
			break;

	if (j < NR_WAY) //hit first
	{
		/* data cross the boundary */
		cache_addr temp2;
		temp2.addr = addr + len - 1;
		uint32_t tag2 = temp2.tag;
		uint32_t index2 = temp2.index;

		if ( index2 == index)
		{
			switch(len)
			{
				case 1: *(uint8_t*)(block[index][j].bib+byte) = (uint8_t)data;
				case 2: *(uint16_t*)(block[index][j].bib+byte) = (uint16_t)data;
				case 4: *(uint32_t*)(block[index][j].bib+byte) = (uint32_t)data;
			}
			dram_write(addr, len, data);
		}
		else
		{
			block[index][j].valid = false;

			for (j = 0; j < NR_WAY; ++j)
				if ( block[index2][j].valid && (block[index2][j].tag == tag2) )
					break;

			if (j < NR_WAY)  //hit again
			{
				block[index2][j].valid = false;
				dram_write(addr, len, data);
			}
			else			//miss again
				dram_write(addr, len, data);
		}
	}
	else			//miss
		dram_write(addr, len, data);
}

#include "common.h"

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

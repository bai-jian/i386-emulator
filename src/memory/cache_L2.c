#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_wirte(hwaddr_t, size_t, uint32_t);

/* Cache L2 Memory Size: 4MB = (4096 Set) * (16 Way/Set) * (64 B/Block) */
#define BIB_WIDTH 6  //BIB: Bytes In a Block
#define WAY_WIDTH 4
#define SET_WIDTH 12

#define NR_BIB (1 << BIB_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)

/* Define [Cache L2] & [Block in the Cache L2] */
struct BLOCK
{
	struct
	{
		uint8_t valid	: 1;
		uint8_t dirty	: 1;
	};
	uint32_t tag;
	uint8_t bib[NR_BIB];
} block[NR_SET][NR_WAY];

/* Define [Cache L2 Address] */
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



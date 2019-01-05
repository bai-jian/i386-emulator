#include "ram/ram.h"

#define COL_WIDTH 10
#define ROW_WIDTH 10
#define BANK_WIDTH 3
#define RANK_WIDTH (27 - COL_WIDTH - ROW_WIDTH - BANK_WIDTH)

#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_BANK (1 << BANK_WIDTH)
#define NR_RANK (1 << RANK_WIDTH)

uint8_t dram[NR_RANK][NR_BANK][NR_ROW][NR_COL];
uint8_t *phy_mem = (void*)dram;

union dram_addr_t
{
	uint32_t addr;
	struct
	{
		uint32_t col : COL_WIDTH;
		uint32_t row : ROW_WIDTH;
		uint32_t bank : BANK_WIDTH;
		uint32_t rank : RANK_WIDTH;
	};
};

struct rowbuf_t
{
	uint8_t buf[NR_COL];
	uint32_t row;
	bool valid;
} rowbufs[NR_RANK][NR_BANK];

void flush_dram()
{
	int i, j;
	for(i = 0; i < NR_RANK; ++i)
		for(j = 0; j < NR_BANK; ++j)
			rowbufs[i][j].valid = false;
}

static void dram_replace(union dram_addr_t dram_addr)
{
	uint32_t rank = dram_addr.rank;
	uint32_t bank = dram_addr.bank;
	uint32_t row = dram_addr.row;

	if (rowbufs[rank][bank].valid && rowbufs[rank][bank].row != row)
		memcpy(dram[rank][bank][rowbufs[rank][bank].row], rowbufs[rank][bank].buf, NR_COL);

	if (!rowbufs[rank][bank].valid || rowbufs[rank][bank].row != row)
	{
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row = row;
		rowbufs[rank][bank].valid = true;
	}
}

void dram_read(phyaddr_t addr, size_t len, uint8_t *data)
{
	union dram_addr_t dram_addr = (union dram_addr_t)addr;
	dram_replace(dram_addr);
	memcpy(data, rowbufs[dram_addr.rank][dram_addr.bank].buf + dram_addr.col, len);
}

void dram_write(phyaddr_t addr, size_t len, uint8_t *data)
{
	union dram_addr_t dram_addr = (union dram_addr_t)addr;	
	dram_replace(dram_addr);
	memcpy(rowbufs[dram_addr.rank][dram_addr.bank].buf + dram_addr.col, data, len);
}


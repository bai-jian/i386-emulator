#include "common.h"
#include "lib/misc.h"

/* Simulate the (main) behavor of DRAM. Although this will lower the performace of NEMU,
 * it makes you clear about how DRAM is read/written.
 */

#define COL_WIDTH 10
#define ROW_WIDTH 10
#define BANK_WIDTH 3
#define RANK_WIDTH (27 - COL_WIDTH - ROW_WIDTH - BANK_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t row	: ROW_WIDTH;
		uint32_t bank	: BANK_WIDTH;
		uint32_t rank	: RANK_WIDTH;
	};
	uint32_t addr;
} dram_addr;


#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_BANK (1 << BANK_WIDTH)
#define NR_RANK (1 << RANK_WIDTH)

#define HW_MEM_SIZE (1 << (COL_WIDTH + ROW_WIDTH + BANK_WIDTH + RANK_WIDTH))

#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

uint8_t dram[NR_RANK][NR_BANK][NR_ROW][NR_COL];
uint8_t *hw_mem = (void *)dram;

typedef struct {
	uint8_t buf[NR_COL];
	int32_t row_idx;
	bool valid;
} RB;

RB rowbufs[NR_RANK][NR_BANK];

void init_dram() {
	int i, j;
	for(i = 0; i < NR_RANK; i ++) {
		for(j = 0; j < NR_BANK; j ++) {
			rowbufs[i][j].valid = false;
		}
	}
}

static void ddr3_read(hwaddr_t addr, void *data) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);

	dram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t rank = temp.rank;
	uint32_t bank = temp.bank;
	uint32_t row = temp.row;
	uint32_t col = temp.col;

	if(!(rowbufs[rank][bank].valid && rowbufs[rank][bank].row_idx == row) ) {
		/* read a row into row buffer */
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row_idx = row;
		rowbufs[rank][bank].valid = true;
	}

	/* burst read */
	memcpy(data, rowbufs[rank][bank].buf + col, BURST_LEN);
}

static void ddr3_write(hwaddr_t addr, void *data, uint8_t *mask) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);

	dram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t rank = temp.rank;
	uint32_t bank = temp.bank;
	uint32_t row = temp.row;
	uint32_t col = temp.col;

	if(!(rowbufs[rank][bank].valid && rowbufs[rank][bank].row_idx == row) ) {
		/* read a row into row buffer */
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row_idx = row;
		rowbufs[rank][bank].valid = true;
	}

	/* burst write */
	memcpy_with_mask(rowbufs[rank][bank].buf + col, data, BURST_LEN, mask);

	/* write back to dram */
	memcpy(dram[rank][bank][row], rowbufs[rank][bank].buf, NR_COL);
}

uint32_t dram_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	
	ddr3_read(addr, temp);

	if( (addr ^ (addr + len - 1)) & ~(BURST_MASK)  ) {
		/* data cross the burst boundary */
		ddr3_read(addr + BURST_LEN, temp + BURST_LEN);
	}

	return *(uint32_t *)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void dram_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	ddr3_write(addr, temp, mask);

	if( (addr ^ (addr + len - 1)) & ~(BURST_MASK)  ) {
		/* data cross the burst boundary */
		ddr3_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}


#include "common.h"
#include "cpu/reg.h"
#include "cpu/mm.h"
#include "ram/ram.h"
#include "cpu/io.h"

#define TLB_SIZE 64
struct translation_lookaside_buffer_t tlb[TLB_SIZE];

void tlb_flush()
{
	int i;
	for (i = 0; i < TLB_SIZE; i++)
		tlb[i].valid = 0;
}

static bool tlb_hit(uint32_t tag, uint32_t *frame)
{
	int i;
	for (i = 0; i < TLB_SIZE; i++)
		if (tlb[i].valid && tag == tlb[i].tag)
			break;
	if (i == TLB_SIZE)
		return false;

	*frame = tlb[i].page_frame_addr;
	return true;
}

static void tlb_update(uint32_t tag, uint32_t frame)
{
	int i;
	for (i = 0; i < TLB_SIZE; i++)
		if (!tlb[i].valid)
			break;
	if (i == TLB_SIZE)
		i = tag % TLB_SIZE;

	tlb[i].valid = 1;
	tlb[i].tag = tag;
	tlb[i].page_frame_addr = frame;
}

#define LNADDR_SIZE 32
#define LNADDR_DIR_OFFSET 22
#define LNADDR_PAGE_OFFSET 12

#define LNADDR_TAG(lnaddr) ((lnaddr_t)(lnaddr) >> LNADDR_PAGE_OFFSET)
#define LNADDR_DIR(lnaddr) ((lnaddr_t)(lnaddr) >> LNADDR_DIR_OFFSET)
#define LNADDR_PAGE(lnaddr) ((lnaddr_t)(lnaddr) >> LNADDR_PAGE_OFFSET & (~0u >> (LNADDR_SIZE - LNADDR_DIR_OFFSET + LNADDR_PAGE_OFFSET)))
#define LNADDR_OFFSET(lnaddr) ((lnaddr_t)(lnaddr) & (~0u >> (LNADDR_SIZE - LNADDR_PAGE_OFFSET)))

#define LNADDR_OFFSET_MAX ((1u << LNADDR_PAGE_OFFSET) - 1)

static phyaddr_t paging(lnaddr_t lnaddr)
{
	if (cpu.CR0_PG == 0)
		return lnaddr;

	uint32_t frame;
	if (tlb_hit(LNADDR_TAG(lnaddr), &frame))
		goto HIT_TLB;

	struct page_table_entry_t pde, pte;
	phyaddr_t pde_phyaddr = (cpu.CR3_pdba << 12) + (LNADDR_DIR(lnaddr) << 2);
	ram_read(pde_phyaddr, 4, (uint8_t *)&pde);
	phyaddr_t pte_phyaddr = (pde.page_frame_addr << 12) + (LNADDR_PAGE(lnaddr) << 2);
	ram_read(pte_phyaddr, 4, (uint8_t *)&pte);
	frame = pte.page_frame_addr;

	tlb_update(LNADDR_TAG(lnaddr), frame);

HIT_TLB:
	return (frame << 12) + LNADDR_OFFSET(lnaddr);
}

static lnaddr_t segmenting(struct segment_selector_t *selector, segment_offset_t offset)
{
	if (cpu.CR0_PE == 0)
		return offset;

	struct segment_descriptor_t descriptor;
	phyaddr_t phaddr = paging((selector->table_indicator ? cpu.LDTR.base : cpu.GDTR.base) + (selector->index << 3));
	ram_read(phaddr, 4, (uint8_t *)&descriptor);
	ram_read(phaddr + 4, 4, (uint8_t *)&descriptor + 4);
	return (descriptor.base_31_24 << 24) + (descriptor.base_23_16 << 16) + descriptor.base_15_0 + offset;
}

void mem_read(swaddr_t addr, size_t len, uint8_t *data)
{
	if (cpu.CR0_PE == 0)
		goto SEGMENTING_OUT;

	addr = segmenting((struct segment_selector_t *)&cpu.sreg[current_sreg], addr);

	if (cpu.CR0_PG == 0)
		goto PAGING_OUT;

	size_t unspanned_len = LNADDR_OFFSET_MAX - LNADDR_OFFSET(addr) + 1;
	if (unspanned_len < len)
	{
		phyaddr_t unspanned_addr = paging(addr);
		
		if (mmio_check(unspanned_addr))
			mmio_read(unspanned_addr, unspanned_len, data);
		else
			ram_read(unspanned_addr, unspanned_len, data);
		
		addr = addr + unspanned_len + 1;
		len = len - unspanned_len;
		data = data + unspanned_len + 1;
	}

	addr = paging(addr);

SEGMENTING_OUT:
PAGING_OUT:
	if (mmio_check(addr))
		mmio_read(addr, len, data);
	else
		ram_read(addr, len, data);
}

void mem_write(swaddr_t addr, size_t len, uint8_t *data)
{
	if (cpu.CR0_PE == 0)
		goto SEGMENTING_OUT;

	addr = segmenting((struct segment_selector_t *)&cpu.sreg[current_sreg], addr);

	if (cpu.CR0_PG == 0)
		goto PAGING_OUT;

	size_t unspanned_len = LNADDR_OFFSET_MAX - LNADDR_OFFSET(addr) + 1;
	if (unspanned_len < len)
	{
		phyaddr_t unspanned_addr = paging(addr);
		
		if (mmio_check(unspanned_addr))
			mmio_write(unspanned_addr, unspanned_len, data);
		else
			ram_write(unspanned_addr, unspanned_len, data);
		
		addr = addr + unspanned_len + 1;
		len = len - unspanned_len;
		data = data + unspanned_len + 1;
	}

	addr = paging(addr);

SEGMENTING_OUT:
PAGING_OUT:
	if (mmio_check(addr))
		mmio_write(addr, len, data);
	else
		ram_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len)
{
	uint8_t temp[4] = {0};
	mem_read(addr, len, temp);
	uint32_t data = (temp[3] << 24) | (temp[2] << 16) | (temp[1] << 8) | temp[0];
	return data;
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{
	uint8_t temp[4];
	temp[0] = data & 0xff;
	temp[1] = (data >> 8) & 0xff;
	temp[2] = (data >> 16) & 0xff;
	temp[3] = (data >> 24) & 0xff;
	mem_write(addr, len, temp);
}

uint32_t instr_fetch(swaddr_t addr, size_t len)
{
	return swaddr_read(addr, len);
}


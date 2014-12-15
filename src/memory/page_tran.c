#include "common.h"

#include "cpu/reg.h"

/* Virtual Memory Space <-> Physical Memory Space */

uint32_t hwaddr_read(hwaddr_t addr, size_t len);
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data);

/* Virtual Memory Space: 4GB = 1K Dir * 1K Page/Dir * 4K B/Page */
#define VPO_WIDTH 12  // VPO: Virutal Page Offset
#define PAGE_WIDTH 10
#define DIR_WIDTH 10
typedef struct
{
	uint32_t vpo  :  VPO_WIDTH;  // Virtual Page Offset
	uint32_t page :  PAGE_WIDTH;
	uint32_t dir  :  DIR_WIDTH;
} VA_t; // VA: Virtual Memory

typedef struct
{
	uint32_t vpo  :  VPO_WIDTH;
	uint32_t tag  :  (PAGE_WIDTH + DIR_WIDTH);
} LA_t; // LA: Logical Memory

/* Define [Page Table] or [Page Directory Table], they shares the same form */
typedef struct
{
	uint32_t P   : 1;  // the page frame is Present
	uint32_t RW  : 1;
	uint32_t US  : 1;
	uint32_t PWT : 1;
	uint32_t PCD : 1;
	uint32_t A   : 1;
	uint32_t D   : 1;
	uint32_t     : 2;
	uint32_t AVL : 3;
	uint32_t PB  : 20;  // Physical Base
} page_t, dir_t;

/* Define [TLB] */
#define NR_TLBE 64
struct
{
	bool valid;
	uint32_t frame;
	uint32_t tag;
} TLB[NR_TLBE];

hwaddr_t page_translate(lnaddr_t addr)
{
	VA_t lnaddr;  *(lnaddr_t*)(&lnaddr) = addr;

	hwaddr_t dir_base = cpu.CR3_pdba;
	dir_t dir_term; *(uint32_t*)(&dir_term) = hwaddr_read((dir_base<<12)+(lnaddr.dir<<2), 4);
	if (dir_term.P == 0)  assert(0);  // The Dir is absent.

	hwaddr_t page_base = dir_term.PB;
	page_t page_term; *(uint32_t*)(&page_term) = hwaddr_read((page_base<<12)+(lnaddr.page<<2), 4);
	if (page_term.P == 0)  assert(0);  // The Page is absent. 
	
	hwaddr_t base = page_term.PB;
	hwaddr_t hwaddr = (base << 12) + lnaddr.vpo;

	return hwaddr;
}

void init_TLB( )
{
	int i;
	for (i = 0; i < NR_TLBE; ++i)
		TLB[i].valid = 0;
}
hwaddr_t page_tlb(lnaddr_t addr)
{
	LA_t lnaddr;  *(lnaddr_t*)(&lnaddr) = addr;

	int i;
	for (i = 0; i < NR_TLBE; ++i)
		if (TLB[i].valid && lnaddr.tag == TLB[i].tag)
			break;

	// Judge hit or miss
	if (i < NR_TLBE)  // hit
	{
		hwaddr_t base = TLB[i].frame;
		hwaddr_t hwaddr = (base << VPO_WIDTH) + lnaddr.vpo;
		return hwaddr;
	}
	else  // miss
	{
		// load or replace
		int tlbe_num;
		for (tlbe_num = 0; tlbe_num < NR_TLBE; ++tlbe_num)
			if ( !TLB[tlbe_num].valid )
				break;
		if (tlbe_num == NR_TLBE)  tlbe_num = 0;

		hwaddr_t hwaddr = page_translate(addr);

		TLB[tlbe_num].valid = 1;
		TLB[tlbe_num].tag = lnaddr.tag;
		TLB[tlbe_num].frame = hwaddr >> VPO_WIDTH;
		return hwaddr;
	}
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
	if (cpu.CR0_PG == 0)
		return hwaddr_read(addr, len);
	else
	{
		VA_t fstb;  *(lnaddr_t*)(&fstb) = addr;
		VA_t lstb;  *(lnaddr_t*)(&lstb) = addr + len - 1;
	
		// Judge whether data crosses the boundary
		if (fstb.page != lstb.page)
		{
			lnaddr_t addr2 = (addr + len - 1) & 0xFFFFFC00;
			size_t len1 = addr2 - addr;
			size_t len2 = len - len1;
			uint32_t data1 = lnaddr_read(addr, len1);
			uint32_t data2 = lnaddr_read(addr2, len2);
			return (data2 << (8*len1)) + data1;  // little endian
		}
		else
		{
			hwaddr_t hwaddr = page_tlb(addr);
			return hwaddr_read(hwaddr, len);
 		}
 	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	if (cpu.CR0_PG == 0)
		return hwaddr_write(addr, len, data);
	else
	{
		VA_t fstb;  *(lnaddr_t*)(&fstb) = addr;
		VA_t lstb;  *(lnaddr_t*)(&lstb) = addr + len - 1;

		// Judge whether data crosses the boundary
		if (fstb.page != lstb.page)
		{
			lnaddr_t addr2 = (addr + len - 1) & 0xFFFFFC00;
			size_t len1 = addr2 - addr;
			size_t len2 = len - len1;
			uint32_t data1 = data << (8*len2) >> (8*len2);
			uint32_t data2 = data >> (8*len1);
			lnaddr_write(addr, len1, data1);
			lnaddr_write(addr2, len2, data2);
			return;
		}
		else
		{
			hwaddr_t hwaddr = page_tlb(addr);
			return hwaddr_write(hwaddr, len, data);
		}
	}
}

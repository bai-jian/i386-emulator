#include "common.h"

/* Virtual Memory Space <- Physical Memory Space */

uint32_t hwaddr_read(hwaddr_t addr, size_t len);
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data);

/* Virtual Memory Space: 4GB = 1K Dir * 1K Page/Dir * 4K B/Page */
#define VPO_WIDTH 12  // VPO: Virutal Page Offset
#define PAGE_WIDTH 10
#define DIR_WIDTH 10
typedef struct
{
	uint32_t VPO  :  12;
	uint32_t PAGE :  10;
	uint32_t DIR  :  10;
} VA_t; // VA_t: Virtual Memory


/* Define [Page Table] or [Page Directory Table] */
#define NR_PAGE (1 << PAGE_WIDTH)
struct
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
} page_table[NR_PAGE];
#define NR_DIR (1 << DIR_WIDTH)
struct
{
	uint32_t P   : 1;
	uint32_t RW  : 1;
	uint32_t US  : 1;
	uint32_t PWT : 1;
	uint32_t PCD : 1;
	uint32_t A   : 1;
	uint32_t D   : 1;
	uint32_t     : 2;
	uint32_t AVL : 3;
	uint32_t PB  : 20;
} dir_table[NR_DIR];


uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
	VA_t fstb;  *(lnaddr_t*)(&fstb) = addr;
	VA_t lstb;  *(lnaddr_t*)(&lstb) = addr + len - 1;
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	return hwaddr_write(addr, len, data);
}

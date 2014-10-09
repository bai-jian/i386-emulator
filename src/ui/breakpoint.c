#include "ui/breakpoint.h"

#include "nemu.h"

#include <stdio.h>

#define NR_BP 32
static BP bp_pool[NR_BP];

static BP* used_head, * free_head;
static int num = 0;

void init_bp_pool() 
{
	used_head = NULL;
	
	free_head = bp_pool;
	int i;
	for(i = 0; i < NR_BP-1; ++i)
		bp_pool[i].next = &bp_pool[i + 1]; 
	bp_pool[i].next = NULL;
}

void new_bp(swaddr_t addr)
{
	BP* newbp = free_head; assert(free_head);
	free_head = free_head->next;

	newbp->NO = ++num;
	newbp->addr = addr;
	newbp->instr = swaddr_read(addr, 1);
	newbp->next = used_head;

	swaddr_write(addr, 1, INT3_CODE);
	
	used_head = newbp;

	return ;
}
void free_bp(int num)
{
	int n = 0;
	BP* bp, * prec;
	for (prec = NULL, bp = used_head; bp; prec = bp, bp = bp->next)
		if (++n == num) break;

	if (bp == NULL)
		printf("No Breakpoint %d!\n", num);
	else if (prec == NULL) {
		used_head = NULL;

		bp->next = free_head;
		free_head = bp; }
	else {
		bp->next = free_head;
		free_head = bp; } 
		
	return ;
}

uint8_t search_bp(swaddr_t swaddr)
{
	BP* p;
	for (p = used_head; ; p = p->next)
		if (p->addr == swaddr) break;
	return p->instr;
}

void print_bp()
{
	BP* p;
	for (p = used_head; p; p = p->next)
		printf("Breakpoint %d at 0x%8X\n", p->NO, p->addr);
}

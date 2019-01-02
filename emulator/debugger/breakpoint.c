#include "debugger/breakpoint.h"
#include "cpu/mm.h"

int bp_state;

#define NR_BP 32
static BP bp_pool[NR_BP];

static BP* used_head, * free_head;
static int num = 0;

void init_bp_pool() 
{
	bp_state = OFF;

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

	printf("New Breakpoint %d at 0x%08X\n", newbp->NO, newbp->addr);

	return ;
}
void free_bp(int num)
{
	if (num == 0)
	{
		while(used_head) {
			BP* p = used_head;
			used_head = used_head->next;
			p->next = free_head; free_head = p; }
	}
	else
	{
		BP* p, * pre;
		for (pre = NULL, p = used_head; p; pre = p, p = p->next)
			if (p->NO == num) break;

		if (p == NULL)
			printf("No Breakpoint %d\n", num);
		else if (pre == NULL) {
		    printf("Delete Breakpoint %d at 0x%08X\n", p->NO, p->addr); 	
			used_head = p->next;
			p->next = free_head; free_head = p; }
		else {
			printf("Delete Breakpoint %d at 0x%08X\n", p->NO, p->addr);
			pre->next = p->next;
			p->next = free_head; free_head = p; }
	}
		
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
		printf("Breakpoint %d at 0x%08X\n", p->NO, p->addr);
}

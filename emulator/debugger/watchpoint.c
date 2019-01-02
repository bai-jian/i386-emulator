#include "debugger/watchpoint.h"
#include "debugger/expr.h"

int WP_STATE = 0; 

#define NR_WP 32
static WP wp_pool[NR_WP];

static WP* used_head, * free_head;
static int wp_num = 0;

void init_wp_pool() 
{
	used_head = NULL;
	
	free_head = wp_pool;
	int i;
	for(i = 0; i < NR_WP-1; ++i)
		wp_pool[i].next = &wp_pool[i + 1]; 
	wp_pool[i].next = NULL;
}

void new_wp(char* e)
{
	++WP_STATE; 

	WP* newwp = free_head; assert(free_head);
	free_head = free_head->next;

	newwp->num = ++wp_num;
	strcpy(newwp->expr, e); 
	newwp->val = expr(e);
	newwp->next = used_head;

	used_head = newwp;

	printf("New Watchpoint %d:  %s\n", newwp->num, newwp->expr);

	return ;
}
void free_wp(int num)
{
	if (num == 0)
	{
		WP_STATE = 0;
		while(used_head) {
			WP* p = used_head;
			used_head = used_head->next;
	  		p->next = free_head; free_head = p; }
	}  
	else
	{  
		--WP_STATE;
		WP* p, * pre;
		for (pre = NULL, p = used_head; p; pre = p, p = p->next)
			if (p->num == num) break;

		if (p == NULL)
			printf("No Watchpoint %d\n", num);
	 	else if (pre == NULL) {
		    printf("Delete Watchpoint %d:  %s\n", p->num, p->expr); 	
			used_head = p->next;
	 		p->next = free_head; free_head = p; }
	  	else {
			printf("Delete Watchpoint %d:  %s\n", p->num, p->expr);
			pre->next = p->next;
			p->next = free_head; free_head = p; }
	}
		
	return ;
}

bool test_wp()
{
	bool changed = false;
	WP* p;
	for (p = used_head; p; p = p->next)
	 {
		size_t val = expr(p->expr);
		if (val != p->val)
	 	{
			if (!changed) changed = true;
			printf("Watchpoint %d \"%s\": %u -> %u\n", \
					p->num, p->expr, p->val, val);
			p->val = val;
		}
	}
	return true;
}

#ifndef __UI_WATCHPOINT_H__
#define __UI_WATCHPOINT_H__

#include "common.h"

extern int WP_STATE;

#define EXPR_MAXSIZE 50
typedef struct watchpoint {
	int num;
	char expr[EXPR_MAXSIZE];
	size_t val;
	struct watchpoint* next;
} WP;

void new_wp(char*);
void free_wp(int);
void test_wp();

#endif

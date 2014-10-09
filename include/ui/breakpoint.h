#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

enum { OFF, TRIG, RESET };
extern int bp_state;

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	size_t addr;
	uint8_t instr;
	struct breakpoint* next;
} BP;

void new_bp(swaddr_t);
void free_bp(int);

uint8_t search_bp(swaddr_t);
void print_bp(); 

#endif

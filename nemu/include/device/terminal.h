#ifndef __DEVICE_TERMINAL_H__
#define __DEVICE_TERMINAL_H__

#include "common.h"

#define CTR_ROW 200
#define CTR_COL 320

typedef void (*timer_irq_t)();
typedef void (*keyboard_irq_t)();

struct terminal_t
{
	timer_irq_t timer_irq;

	uint8_t *keyboard_scancode;
	keyboard_irq_t keyboard_irq;

	uint8_t *vga_vmem;
	bool *vga_vmem_dirty;
	bool *vga_vmem_line_dirty;
};

struct terminal_t terminal;

void terminal_init();
void terminal_clear();

#endif

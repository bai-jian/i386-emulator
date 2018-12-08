#ifndef __X86_H__
#define __X86_H__

#include "common.h"

static inline uint8_t
in_byte(uint16_t port) {
	uint8_t data;
	asm volatile("in %1, %0" : "=a"(data) : "d"(port));
	return data;
}

static inline void
out_byte(uint16_t port, int8_t data) {
	asm volatile("out %%al, %%dx" : : "a"(data), "d"(port));
}

static inline void
sti(void) {
	asm volatile("sti");
}

static inline void
cli(void) {
	asm volatile("cli");
}

static inline void
wait_intr() {
	asm volatile("hlt");
}

#endif

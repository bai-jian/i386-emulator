#include "common.h"

#define WRITEBACK_TIME  1  /* writeback cache for every 1 second */
#define HZ 100

void cache_init(void);
void cache_writeback(void);
uint8_t read_byte(uint32_t);
void write_byte(uint32_t, uint8_t);

void
init_ide(void) {
	cache_init();
}

/* The loader acts as a monolithic kernel, therefore we do not need
 * to translate the address ``buf'' from the user process to a physical
 * one, which is necessary for a microkernel.
 */
void ide_read(uint8_t *buf, uint32_t offset, uint32_t len) {
	uint32_t i;
	for (i = 0; i < len; i ++) {
		buf[i] = read_byte(offset + i);
	}
}

void ide_write(uint8_t *buf, uint32_t offset, uint32_t len) {
	uint32_t i;
	for (i = 0; i < len; i ++) {
		write_byte(offset + i, buf[i]);
	}
}

void
ide_writeback(void) {
	static uint32_t counter = 0;
	counter ++;
	if (counter == WRITEBACK_TIME * HZ) {
		cache_writeback();
		counter = 0;
	}
}

#include "common.h"
#include "x86.h"

#define IDE_PORT_CTRL   0x3F6
#define IDE_PORT_BASE   0x1F0

static void waitdisk() {
	while ( (in_byte(IDE_PORT_BASE + 7) & (0x80 | 0x40)) != 0x40);
}

static void
ide_prepare(uint32_t sector) {
	waitdisk();

//	out_byte(IDE_PORT_CTRL, 0);
	out_byte(IDE_PORT_BASE + 2, 1);
	out_byte(IDE_PORT_BASE + 3, sector & 0xFF);
	out_byte(IDE_PORT_BASE + 4, (sector >> 8) & 0xFF);
	out_byte(IDE_PORT_BASE + 5, (sector >> 16) & 0xFF);
	out_byte(IDE_PORT_BASE + 6, 0xE0 | ((sector >> 24) & 0xFF));
}

static inline void
issue_read() {
	out_byte(IDE_PORT_BASE + 7, 0x20);
}

static inline void
issue_write() {
	out_byte(IDE_PORT_BASE + 7, 0x30);
}

void
disk_do_read(void *buf, uint32_t sector) {
	int i;

	ide_prepare(sector);
	issue_read();

	waitdisk();
	for (i = 0; i < 512 / sizeof(uint32_t); i ++) {
		*(((uint32_t*)buf) + i) = in_long(IDE_PORT_BASE);
	}
}

void
disk_do_write(void *buf, uint32_t sector) {
	int i;

	ide_prepare(sector);
	issue_write();

	for (i = 0; i < 512 / sizeof(uint32_t); i ++) {
		out_long(IDE_PORT_BASE, *(((uint32_t*)buf) + i));
	}
}

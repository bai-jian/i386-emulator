#ifndef __DEVICE_VGA_H__
#define __DEVICE_VGA_H__

#include "common.h"

#define KEYBOARD_PORT_BASE 0x60
#define KEYBOARD_PORT_SIZE 1
#define KEYBOARD_IRQ 1

void vga_register();
void vga_read_handler(offset_t offset, size_t len, uint32_t *data);
void vga_write_handler(offset_t offset, size_t len, uint32_t data);

#endif

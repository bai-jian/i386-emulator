#ifndef __DEVICE_VGA_H__
#define __DEVICE_VGA_H__

#include "common.h"

void vga_register();
void vga_dac_read_handler(offset_t offset, size_t len, uint32_t *data);
void vga_dac_write_handler(offset_t offset, size_t len, uint32_t data);
void vga_crtc_read_handler(offset_t offset, size_t len, uint32_t *data);
void vga_crtc_write_handler(offset_t offset, size_t len, uint32_t data);	
void vga_vmem_read_handler(offset_t offset, size_t len, uint8_t *data);
void vga_vmem_write_handler(offset_t offset, size_t len, uint8_t *data);

#endif

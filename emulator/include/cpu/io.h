#ifndef __CPU_IO_H__
#define __CPU_IO_H__

#include "common.h"

typedef void (*pio_read_cb_t)(offset_t offset, size_t len, uint32_t *data);
typedef void (*pio_write_cb_t)(offset_t offset, size_t len, uint32_t data);
typedef void (*mmio_read_cb_t)(offset_t offset, size_t len, uint8_t *data);
typedef void (*mmio_write_cb_t)(offset_t offset, size_t len, uint8_t *data);

void pio_register(ioaddr_t addr, size_t len, pio_read_cb_t read_cb, pio_write_cb_t write_cb);
void pio_read(ioaddr_t addr, size_t len, uint32_t *data);
void pio_write(ioaddr_t addr, size_t len, uint32_t data);

void mmio_register(hwaddr_t addr, size_t len, mmio_read_cb_t read_cb, mmio_write_cb_t write_cb);
bool mmio_check(hwaddr_t addr);
void mmio_read(hwaddr_t addr, size_t len, uint8_t *data);
void mmio_write(hwaddr_t addr, size_t len, uint8_t *data);

#endif

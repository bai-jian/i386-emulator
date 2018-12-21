#ifndef __DEVICE_SERIAL_H__
#define __DEVICE_SERIAL_H__

#include "common.h"

#define SERIAL_PORT_BASE 0x3F8
#define SERIAL_PORT_SIZE 8
#define SERIAL_OFFSET_CH 0
#define SERIAL_OFFSET_LS 5 /* line status register */

void serial_register();
void serial_read_handler(offset_t offset, size_t len, uint32_t *data);
void serial_write_handler(offset_t offset, size_t len, uint32_t data);

#endif

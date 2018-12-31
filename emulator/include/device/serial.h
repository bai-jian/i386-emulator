#ifndef __DEVICE_SERIAL_H__
#define __DEVICE_SERIAL_H__

#include "common.h"

void serial_register();
void serial_read_handler(offset_t offset, size_t len, uint32_t *data);
void serial_write_handler(offset_t offset, size_t len, uint32_t data);

#endif

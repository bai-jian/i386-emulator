#ifndef __DEVICE_IDE_H__
#define __DEVICE_IDE_H__

#include "common.h"

void ide_register();
void ide_read_handler(offset_t offset, size_t len, uint32_t *data);
void ide_write_handler(offset_t offset, size_t len, uint32_t data);
void ide_irq();

#endif

#ifndef __DEVICE_IDE_H__
#define __DEVICE_IDE_H__

#include "common.h"

#define IDE_PORT_BASE 0x1F0
#define IDE_PORT_SIZE 8
#define IDE_IRQ 14

void ide_register();
void ide_read_handler(offset_t offset, size_t len, uint32_t *data);
void ide_write_handler(offset_t offset, size_t len, uint32_t data);
void ide_irq();

#endif

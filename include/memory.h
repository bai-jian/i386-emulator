#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#define HW_MEM_SIZE (128 * 1024 * 1024)

extern uint8_t *hw_mem;

#define hwa_to_va(p) ((void *)(hw_mem + (unsigned)p))
#define va_to_hwa(p) ((hwaddr_t)((void *)p - (void *)hw_mem))

uint32_t swaddr_read(swaddr_t, size_t);
void swaddr_write(swaddr_t, size_t, uint32_t);

#endif

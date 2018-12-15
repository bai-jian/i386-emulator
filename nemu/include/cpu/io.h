#ifndef __CPU_IO_H__
#define __CPU_IO_H__

#include "common.h"

#define PIO_SPACE_MAX (65536)
#define PIO_NR_DEVICE (8)

typedef void (*pio_callback_t)(ioaddr_t addr, size_t len, bool is_write);
typedef void (*pio_read_t)(ioaddr_t addr, size_t len, uint32_t *data);
typedef void (*pio_write_t)(ioaddr_t addr, size_t len, uint32_t data);
typedef uint8_t *(*pio_register_t)(ioaddr_t addr, size_t len, pio_callback_t callback);

struct pio_device_t
{
	ioaddr_t low;
	ioaddr_t high;
	pio_callback_t callback;
};

struct pio_t
{
	uint8_t space[PIO_SPACE_MAX];

	struct pio_device_t devices[PIO_NR_DEVICE];
	int nr_device;

	pio_read_t read;
	pio_write_t write;
	pio_register_t register_device;
};

#define MMIO_SPACE_MAX (256 * 1024)
#define MMIO_NR_DEVICE (8)

typedef void (*mmio_callback_t)(hwaddr_t addr, size_t len, bool is_write);
typedef bool (*mmio_check_t)(hwaddr_t addr);
typedef void (*mmio_read_t)(hwaddr_t addr, size_t len, uint32_t *data);
typedef void (*mmio_write_t)(hwaddr_t addr, size_t len, uint32_t data);
typedef uint8_t *(*mmio_register_t)(hwaddr_t addr, size_t len, mmio_callback_t callback);

struct mmio_device_t
{
	hwaddr_t low;
	hwaddr_t high;
	uint8_t *space;
	mmio_callback_t callback;
};

struct mmio_t
{
	uint8_t space[MMIO_SPACE_MAX];
	uint32_t space_free_idx;

	struct mmio_device_t devices[MMIO_NR_DEVICE];
	int nr_device;

	mmio_check_t check;
	mmio_read_t read;
	mmio_write_t write;
	mmio_register_t register_device;
};

struct pio_t i386_pio;
struct mmio_t i386_mmio;

#endif

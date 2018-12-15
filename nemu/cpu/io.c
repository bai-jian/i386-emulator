#include "common.h"
#include "cpu/io.h"

static void _pio_read(ioaddr_t addr, size_t len, uint32_t *data);
static void _pio_write(ioaddr_t addr, size_t len, uint32_t data);
static uint8_t* _pio_register(ioaddr_t addr, size_t len, pio_callback_t callback);

static bool _mmio_check(hwaddr_t addr);
static void _mmio_read(hwaddr_t addr, size_t len, uint32_t *data);
static void _mmio_write(hwaddr_t addr, size_t len, uint32_t data);
static uint8_t* _mmio_register(hwaddr_t addr, size_t len, mmio_callback_t callback);

struct pio_t i386_pio =
{
	.nr_device = 0,
	.read = _pio_read,
	.write = _pio_write,
	.register_device = _pio_register
};

struct mmio_t i386_mmio =
{
	.space_free_idx = 0,
	.nr_device = 0,
	.check = _mmio_check,
	.read = _mmio_read,
	.write = _mmio_write,
	.register_device = _mmio_register
};

void _pio_read(ioaddr_t addr, size_t len, uint32_t *data)
{
	assert(len == 1 || len == 2 || len == 4);
	assert(addr + len - 1 < PIO_SPACE_MAX);

	struct pio_device_t *device = NULL;
	for (device = i386_pio.devices; device < i386_pio.devices + i386_pio.nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == i386_pio.devices + i386_pio.nr_device)
		return;

	memcpy(data, i386_pio.space + addr, len);
	
	if (device->callback)
		device->callback(addr, len, false);
}

void _pio_write(ioaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	assert(addr + len - 1 < PIO_SPACE_MAX);

	struct pio_device_t *device = NULL;
	for (device = i386_pio.devices; device < i386_pio.devices + i386_pio.nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == i386_pio.devices + i386_pio.nr_device)
		return;

	memcpy(i386_pio.space + addr, &data, len);

	if (device->callback)
		device->callback(addr, len, true);
}

static uint8_t* _pio_register(ioaddr_t addr, size_t len, pio_callback_t callback)
{
	assert(i386_pio.nr_device < PIO_NR_DEVICE);
	assert(addr + len -1 < PIO_SPACE_MAX);

	struct pio_device_t *device = i386_pio.devices + i386_pio.nr_device;
	device->low = addr;
	device->high = addr + len - 1;
	device->callback = callback;
	++ i386_pio.nr_device;
	
	return i386_pio.space + addr;
}

static bool _mmio_check(hwaddr_t addr)
{
	struct mmio_device_t *device = NULL;
	for (device = i386_mmio.devices; device < i386_mmio.devices + i386_mmio.nr_device; device++)
		if (addr >= device->low && addr <= device->high)
			return true;
	return false;
}

static void _mmio_read(hwaddr_t addr, size_t len, uint32_t *data)
{
	assert(len == 1 || len == 2 || len == 4);

	struct mmio_device_t *device = NULL;
	for (device = i386_mmio.devices; device < i386_mmio.devices + i386_mmio.nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == i386_mmio.devices + i386_mmio.nr_device)
		return;

	memcpy(data, device->space + (addr - device->low), len);

	if (device->callback)
		device->callback(addr, len, false);
}

static void _mmio_write(hwaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);

	struct mmio_device_t *device = NULL;
	for (device = i386_mmio.devices; device < i386_mmio.devices + i386_mmio.nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == i386_mmio.devices + i386_mmio.nr_device)
		return;

	memcpy(device->space + (addr - device->low), &data, len);

	if (device->callback)
		device->callback(addr, len, true);
}

static uint8_t* _mmio_register(hwaddr_t addr, size_t len, mmio_callback_t callback)
{
	assert(i386_mmio.nr_device < MMIO_NR_DEVICE);
	assert(i386_mmio.space_free_idx + len -1 < MMIO_SPACE_MAX);

	struct mmio_device_t *device = i386_mmio.devices + i386_mmio.nr_device;
	device->low = addr;
	device->high = addr + len - 1;
	device->space = i386_mmio.space + i386_mmio.space_free_idx;
	device->callback = callback;

	i386_mmio.space_free_idx += len;
	++i386_mmio.nr_device;

	return device->space;
}


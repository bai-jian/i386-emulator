#include "common.h"
#include "cpu/io.h"

#define NR_DEVICE (8)

struct pio_device_t
{
	ioaddr_t low;
	ioaddr_t high;
	pio_read_cb_t read_cb;
	pio_write_cb_t write_cb;
};

static struct pio_device_t pio_devices[NR_DEVICE];
static int pio_nr_device = 0;

struct mmio_device_t
{
	phyaddr_t low;
	phyaddr_t high;
	mmio_read_cb_t read_cb;
	mmio_write_cb_t write_cb;
};

static struct mmio_device_t mmio_devices[NR_DEVICE];
static int mmio_nr_device = 0;

void pio_register(ioaddr_t addr, size_t len, pio_read_cb_t read_cb, pio_write_cb_t write_cb)
{
	struct pio_device_t *device = pio_devices + pio_nr_device++;
	device->low = addr;
	device->high = addr + len - 1;
	device->read_cb = read_cb;
	device->write_cb = write_cb;
}

void pio_read(ioaddr_t addr, size_t len, uint32_t *data)
{
	struct pio_device_t *device = NULL;
	for (device = pio_devices; device < pio_devices + pio_nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == pio_devices + pio_nr_device)
		return;

	if (device->read_cb)
		device->read_cb(addr - device->low, len, data);
}

void pio_write(ioaddr_t addr, size_t len, uint32_t data)
{
	struct pio_device_t *device = NULL;
	for (device = pio_devices; device < pio_devices + pio_nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == pio_devices + pio_nr_device)
		return;

	if (device->write_cb)
		device->write_cb(addr - device->low, len, data);
}

void mmio_register(phyaddr_t addr, size_t len, mmio_read_cb_t read_cb, mmio_write_cb_t write_cb)
{
	struct mmio_device_t *device = mmio_devices + mmio_nr_device++;
	device->low = addr;
	device->high = addr + len - 1;
	device->read_cb = read_cb;
	device->write_cb = write_cb;
}

bool mmio_check(phyaddr_t addr)
{
	struct mmio_device_t *device = NULL;
	for (device = mmio_devices; device < mmio_devices + mmio_nr_device; device++)
		if (addr >= device->low && addr <= device->high)
			return true;
	return false;
}

void mmio_read(phyaddr_t addr, size_t len, uint8_t *data)
{
	struct mmio_device_t *device = NULL;
	for (device = mmio_devices; device < mmio_devices + mmio_nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == mmio_devices + mmio_nr_device)
		return;

	if (device->read_cb)
		device->read_cb(addr - device->low, len, data);
}

void mmio_write(phyaddr_t addr, size_t len, uint8_t *data)
{
	struct mmio_device_t *device = NULL;
	for (device = mmio_devices; device < mmio_devices + mmio_nr_device; device++)
		if (addr >= device->low && addr + len - 1 <= device->high)
			break;
	if (device == mmio_devices + mmio_nr_device)
		return;

	if (device->write_cb)
		device->write_cb(addr - device->low, len, data);
}


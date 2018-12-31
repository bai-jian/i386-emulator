#include "device/vga.h"
#include "device/terminal.h"
#include "cpu/io.h"

#define VGA_DAC_PORT_BASE 0x3c8
#define VGA_DAC_PORT_SIZE 2
#define VGA_DAC_OFFSET_INDEX 0
#define VGA_DAC_OFFSET_DATA 1

#define VGA_CRTC_PORT_BASE 0x3d4
#define VGA_CRTC_PORT_SIZE 2
#define VGA_CRTC_OFFSET_INDEX 0
#define VGA_CRTC_OFFSET_DATA 1

#define VGA_VMEM_MM_BASE 0xa0000
#define VGA_VMEM_MM_SIZE 0x20000

#define VGA_VMEM_ROW 200
#define VGA_VMEM_COL 320
#define VGA_VMEM_SIZE (VGA_VMEM_ROW * VGA_VMEM_COL)

struct vga_t
{
	uint8_t vga_crtc_regs[19];

	uint8_t vmem[VGA_VMEM_SIZE];
	bool vmem_dirty;
	bool vmem_line_dirty[VGA_VMEM_ROW];
};

static struct vga_t vga;

void vga_register()
{
	pio_register(VGA_DAC_PORT_BASE, VGA_DAC_PORT_SIZE, vga_dac_read_handler, vga_dac_write_handler);
	pio_register(VGA_CRTC_PORT_BASE, VGA_CRTC_PORT_SIZE, vga_crtc_read_handler, vga_crtc_write_handler);
	mmio_register(VGA_VMEM_MM_BASE, VGA_VMEM_MM_SIZE, vga_vmem_read_handler, vga_vmem_write_handler);

	terminal.vga_vmem = vga.vmem;
	terminal.vga_vmem_dirty = &vga.vmem_dirty;
	terminal.vga_vmem_line_dirty = vga.vmem_line_dirty;
}

void vga_dac_read_handler(offset_t offset, size_t len, uint32_t *data)
{
	assert(0);
}

void vga_dac_write_handler(offset_t offset, size_t len, uint32_t data)
{
	assert(0);
}

void vga_crtc_read_handler(offset_t offset, size_t len, uint32_t *data)
{
	assert(0);
}

void vga_crtc_write_handler(offset_t offset, size_t len, uint32_t data)
{
	assert(0);
}

void vga_vmem_read_handler(offset_t offset, size_t len, uint8_t *data)
{
	if (offset + len <= VGA_VMEM_SIZE)
	{
		memcpy(data, vga.vmem + offset, len);
	}
}

void vga_vmem_write_handler(offset_t offset, size_t len, uint8_t *data)
{
	if (offset + len <= VGA_VMEM_SIZE)
	{
		memcpy(vga.vmem + offset, data, len);
		vga.vmem_dirty = true;
		vga.vmem_line_dirty[offset / VGA_VMEM_COL] = true;
	}
}


#include "device/ide.h"
#include "cpu/io.h"
#include "cpu/int.h"

struct disk_t
{
	uint8_t port[IDE_PORT_SIZE];

	FILE *fp;
	uint32_t bytes;
};

static disk_t disk;
static bool ide_write;

/*
static uint8_t *ide_port_base;

static uint32_t sector, disk_idx;
static uint32_t byte_cnt;
static bool ide_write;
static FILE *disk_fp;
*/

void ide_register()
{
	pio_register(IDE_PORT_BASE, IDE_PORT_SIZE, ide_read_handler, ide_write_handler);

	disk.port[7] = 0x40;

	extern char* exec_file;
	disk.fp = fopen(exec_file, "r+");
	assert(disk_fp);
}

void ide_read_handler(offset_t offset, size_t len, uint32_t *data)
{
	assert(disk.bytes <= 512);

	if(offset == 0 && len == 4)
	{
		assert(!ide_write);
		fread(disk.port, 4, 1, disk.fp);
	
		disk.bytes += 4;
		if(disk.bytes == 512)
		{
			disk.port[7] = 0x40;
		}
	}
}

void ide_write_handler(offset_t offset, size_t len, uint32_t data)
{
	assert(disk.bytes <= 512);

	if(offset == 0 && len == 4)
	{
		assert(ide_write);
		fwrite(disk.port, 4, 1, disk.fp);

		disk.bytes += 4;
		if(disk.bytes == 512)
		{
			disk.port[7] = 0x40;
			i8259_irq(IDE_IRQ);
		}
	}
	else if(offset == 7 && len == 1)
	{
		if (disk.port[7] != 0x20 && disk.port[7] != 0x30)
			return;
		
		uint32_t sector = (disk.port[6] & 0x1f) << 24 | disk.port[5] << 16 | disk.port[4] << 8 | disk.port[3];
		fseek(disk.fp, sector << 9, SEEK_SET);

		disk.bytes = 0;

		if(disk.port[7] == 0x20)
		{
			ide_write = false;
			fread(disk.port, 4, 1, disk.fp);
			disk.port[7] = 0x40;
			i8259_irq(IDE_IRQ);
		}
		else if(disk.port[7] == 0x30)
		{
			ide_write = true;
		}
		else {
			assert(0);
		}
	}
}

void ide_irq()
{
	i8259_irq(IDE_IRQ);
}


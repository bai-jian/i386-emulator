#include "device/serial.h"
#include "cpu/io.h"

void serial_register()
{
	pio_register(SERIAL_PORT_BASE, SERIAL_PORT_SIZE, serial_read_handler, serial_write_handler);
}

void serial_read_handler(offset_t offset, size_t len, uint32_t *data)
{
	if (offset == SERIAL_OFFSET_LS && len == 1)
	{
		/* the status is always free */
		*data = 0x20;
	}
}

void serial_write_handler(offset_t offset, size_t len, uint32_t data)
{
	if(offset == SERIAL_OFFSET_CH && len == 1)
	{
		/* bind the serial port with the host stdout in nemu. */
		char c = data;
		putc(c, stdout);
		if(c == '\n')
			fflush(stdout);
	}
}


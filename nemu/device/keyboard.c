#include "device/keyboard.h"
#include "cpu/io.h"
#include "cpu/int.h"

static uint8_t *i8042_data_port_base;
static bool newkey;

struct keyboard_t
{
	uint8_t port[KEYBOARD_PORT_SIZE];
	bool pressed;
};

static struct keyboard_t keyboard;

void keyboard_register()
{
	pio_register(KEYBOARD_PORT_BASE, KEYBOARD_PORT_SIZE, keyboard_read_handler, keyboard_write_handler);

	keyboard.pressed = false;
}

void keyboard_read_handler(offset_t offset, size_t len, uint32_t *data)
{
	if (offset == 0 && len == 1)
	{
		*data = keyboard.port[0];
	}
}

void keyboard_write_handler(offset_t offset, size_t len, uint32_t data)
{
	return;
}

void keyboard_irq(uint8_t scancode)
{
	if(nemu_state == RUNNING)
	{
		if (keyboard.pressed)
			return;
		keyboard.port[0] = scancode;
		keyboard.pressed = true;
		i8259_irq(KEYBOARD_IRQ);
	}
}

void keyboard_intr(uint8_t scancode) {
	if(nemu_state == RUNNING && newkey == false) {
		i8042_data_port_base[0] = scancode;
		i8259_irq_raise(KEYBOARD_IRQ);
		newkey = true;
	}
}

void i8042_io_handler(ioaddr_t addr, size_t len, bool is_write) {
	if(!is_write) {
		newkey = false;
	}
}


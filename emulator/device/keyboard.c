#include "device/keyboard.h"
#include "device/terminal.h"
#include "cpu/io.h"
#include "cpu/int.h"

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

	terminal.keyboard_irq = keyboard_irq;
}

void keyboard_read_handler(offset_t offset, size_t len, uint32_t *data)
{
	if (offset == 0 && len == 1)
	{
		*data = keyboard.port[0];
		keyboard.pressed = false;
	}
}

void keyboard_write_handler(offset_t offset, size_t len, uint32_t data)
{
	assert(0);
}

void keyboard_irq()
{
	if(nemu_state == RUNNING)
	{
		if (keyboard.pressed)
			return;
		keyboard.port[0] = *terminal.keyboard_scancode;
		keyboard.pressed = true;
		i8259_irq(KEYBOARD_IRQ);
	}
}


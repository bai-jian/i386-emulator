#include "device/timer.h"
#include "device/terminal.h"
#include "cpu/int.h"

#define TIMER_IRQ 0

void timer_register()
{
	terminal.timer_irq = timer_irq;
}

void timer_irq()
{
	if(nemu_state == RUNNING)
	{
		i8259_irq(TIMER_IRQ);
	}
}


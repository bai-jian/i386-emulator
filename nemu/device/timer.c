#include "device/timer.h"
#include "cpu/int.h"

void timer_irq()
{
	if(nemu_state == RUNNING)
	{
		i8259_irq(TIMER_IRQ);
	}
}


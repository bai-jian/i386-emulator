#include "common.h"
#include "x86.h"

#define SERIAL_PORT  0x3F8

static inline int serial_idle(void)
{
	return (in_byte(SERIAL_PORT + 5) & 0x20) != 0;
}

void serial_printc(char ch)
{
	while (!serial_idle());
	out_byte(SERIAL_PORT, ch);
}

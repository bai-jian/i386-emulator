#ifndef __CPU_INT_H__
#define __CPU_INT_H__

#include "common.h"

struct gate_descriptor_t
{
	uint32_t offset_15_0     : 16;
	uint32_t selector        : 16;
	uint32_t                 : 8;
	uint32_t type            : 4;
	uint32_t system          : 1;
	uint32_t privilege_level : 2;
	uint32_t present         : 1;
	uint32_t offset_31_16    : 16;
};

void int_handle(uint8_t intid);

void i8259_irq(uint8_t irqno);
void i8259_irq_query(uint8_t *irqno);
void i8259_irq_ack();

#endif

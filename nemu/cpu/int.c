#include "common.h"
#include "cpu/reg.h"
#include "cpu/int.h"
#include "memory.h"

void int_handle(uint8_t intid)
{
	uint8_t offset = intid;
	uint32_t base = cpu.IDTR.base;
	uint32_t lnaddr = base + (offset << 3);

	uint64_t descriptor_l = lnaddr_read(lnaddr    , 4);
	uint64_t descriptor_h = lnaddr_read(lnaddr + 4, 4);
	uint64_t descriptor   = (descriptor_h << 32) + descriptor_l;
	struct gate_descriptor_t gate_desc = *(struct gate_descriptor_t *)(&descriptor);
	
	// Push EFLAGS, CS, EIP
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.eflags);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.CS);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.eip);

	// Load CS, EIP with the Gate Descriptor
	cpu.CS = gate_desc.selector;
	cpu.eip = ((uint32_t)gate_desc.offset_31_16 << 16) + (uint32_t)gate_desc.offset_15_0;
}

#define IRQ_BASE 32
#define NO_INTR -1
#define MASK(n) (1 << (n))

typedef struct {
	uint8_t IRR;		/* interrupt request register */
	uint8_t ISR;		/* interrupt service register */
	uint8_t IMR;		/* interrupt mask register */
	int8_t highest_irq;
} i8259;

static i8259 master, slave;
static uint8_t intr_NO;

/* find first '1' */
static const char ffo_table[] = {
	-1, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/* i8259 internal */
static void do_i8259() {
	int8_t master_irq = master.highest_irq;
	if(master_irq == NO_INTR) {
		cpu.INTR = false;
		return;
	}
	else if(master_irq == 2) {
		assert(slave.highest_irq != NO_INTR);
		master_irq = 8 + slave.highest_irq;
	}

	intr_NO = master_irq + IRQ_BASE;
	cpu.INTR = true;
}

/* device interface */
void i8259_irq(uint8_t n)
{
	assert(n >= 0 && n < 16);
	if(n < 8) {
		master.IRR |= MASK(n);
	}
	else {
		n -= 8;
		slave.IRR |= MASK(n);
		master.IRR |= MASK(2);

		slave.highest_irq = ffo_table[slave.IRR & ~slave.IMR];
	}

	master.highest_irq = ffo_table[master.IRR & ~master.IMR];

	do_i8259();
}

/* CPU interface */
void i8259_irq_query(uint8_t *irqno)
{
	*irqno = intr_NO;
}

void i8259_irq_ack()
{
	if(intr_NO == NO_INTR) {
		return;
	}

	int n = intr_NO - IRQ_BASE;
	if(n < 8) {
		master.IRR &= ~MASK(n);
	}
	else {
		n -= 8;
		slave.IRR &= ~MASK(n);
		master.IRR &= ~MASK(2);

		slave.highest_irq = ffo_table[slave.IRR & ~slave.IMR];
	}

	master.highest_irq = ffo_table[master.IRR & ~master.IMR];

	do_i8259();
}


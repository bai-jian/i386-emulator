#include "exec/helper.h"


#include "memory.h"
#include "desc_table.h"
#include "cpu/reg.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);
void swaddr_write(swaddr_t addr, size_t len, uint32_t data);
void raise_intr(uint8_t no);
make_helper(INT)
{
//	Log("cs %x  eip %x\n", cpu.CS, cpu.eip);
	uint8_t instr_len = 2;
	uint8_t imm = instr_fetch(eip + 1, 1);
/*
	// Find the Gate Descriptor
	uint32_t base = cpu.IDTR.base;
	uint8_t offset = instr_fetch(eip + 1, 1);
	uint32_t lnaddr = base + (offset << 3);

	uint64_t descriptor_l = lnaddr_read(lnaddr    , 4);
	uint64_t descriptor_h = lnaddr_read(lnaddr + 4, 4);
	uint64_t descriptor   = (descriptor_h << 32) + descriptor_l;
	GateDesc_t gate_desc = *(GateDesc_t*)(&descriptor);

	// Push EFLAGS, CS, EIP
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.eflags);
	cpu.esp -= 2;  swaddr_write(cpu.esp, 2, cpu.CS);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.eip + instr_len);

	// Load CS, EIP with the Gate Descriptor
	cpu.CS = gate_desc.selector;
	cpu.eip = ((uint32_t)gate_desc.off_h << 16) + (uint32_t)gate_desc.off_l - instr_len;
*/
	cpu.eip += instr_len;
	raise_intr(imm);
	cpu.eip -= instr_len;
	print_asm("int    $0x%x", imm);

	return instr_len;
}

void raise_intr(uint8_t no)
{
//	Log("no: 0x%x\n", no);
	uint8_t offset = no;
	uint32_t base = cpu.IDTR.base;
	uint32_t lnaddr = base + (offset << 3);

	uint64_t descriptor_l = lnaddr_read(lnaddr    , 4);
	uint64_t descriptor_h = lnaddr_read(lnaddr + 4, 4);
	uint64_t descriptor   = (descriptor_h << 32) + descriptor_l;
	GateDesc_t gate_desc = *(GateDesc_t*)(&descriptor);
	
	// Push EFLAGS, CS, EIP
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.eflags);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.CS);
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, cpu.eip);

	// Load CS, EIP with the Gate Descriptor
	cpu.CS = gate_desc.selector;
	cpu.eip = ((uint32_t)gate_desc.off_h << 16) + (uint32_t)gate_desc.off_l;
//	Log("cs %x, eip %x\n", cpu.CS, cpu.eip);
}

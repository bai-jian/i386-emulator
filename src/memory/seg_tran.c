#include "common.h"


#include "cpu/reg.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);
void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data);


lnaddr_t segment_translate(swaddr_t addr, size_t len, uint8_t cur_sreg);

uint32_t swaddr_read(swaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
	return lnaddr_write(lnaddr, len, data);
}
uint32_t instr_fetch(swaddr_t addr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, len, R_CS);
	return lnaddr_read(lnaddr, len);
}


lnaddr_t segment_translate(swaddr_t addr, size_t len, uint8_t cur_sreg)
{
	// selector:      16...3   2   1 0
	//                index    TI  RPL
	//
	//
	// descriptor:     63...56   55  54  53  52     51...48     47  46 45  44  43...41  40   39...16     15...0
	//               base[31-24] G   D   0   AVL  limit[19-16]  P    DPL   S    TYPE    A   base[23-0] limit[15-0] 


	uint8_t RPL = cpu.sreg[cur_sreg].RPL;
	uint8_t TI = cpu.sreg[cur_sreg].TI;
	uint32_t index = cpu.sreg[cur_sreg].index;

	uint64_t descriptor = *( (uint64_t*)((TI == 0) ? cpu.GDTR : cpu.LDTR) + index );
	uint8_t DPL = (descriptor >> 45) & 0x03;


	/* Priviledge */
	// CPL: Current Privilege Level = CS RPL
	// RPL: Requestor's Privilege Level
	// DPL: Descriptor Privilege Level
	//
	// An changing-over of some data segment is legal
	//         iff    (1) target_descriptor.DPL >= requestor.RPL  &&
	//                (2) target_descriptor.DPL >= current_program.RPL
	uint8_t CPL = cpu.sreg[R_CS].RPL;
	if ( !(DPL >= RPL && DPL >= CPL) )  assert(0);


	uint32_t base = ( (descriptor >> 16) & 0x00FFFFFF ) + (descriptor >> 56 << 24);
	uint32_t lnaddr = base + addr;

	return lnaddr;
}

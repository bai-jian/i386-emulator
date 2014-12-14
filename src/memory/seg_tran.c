#include "common.h"

#include "cpu/reg.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);
uint32_t hwaddr_read(hwaddr_t addr, size_t len);

lnaddr_t segment_translate(swaddr_t addr, size_t len, uint8_t cur_sreg)
{
	// selector:      16...3   2   1 0
	//                index    TI  RPL
	//
	//
	// descriptor:     63...56   55  54  53  52     51...48     47  46 45  44  43...41  40   39...32     31...16     15...0
	//               base[31-24] G   D   0   AVL  limit[19-16]  P    DPL   S    TYPE    A   base[23-16] base[15-0] limit[15-0] 


	uint8_t RPL = cpu.sreg[cur_sreg].RPL;
	uint8_t TI = cpu.sreg[cur_sreg].TI;
	uint32_t index = cpu.sreg[cur_sreg].index;

	
	uint32_t descriptor_l, descriptor_h;
	if (TI == 0)
	{
		descriptor_l = hwaddr_read(cpu.GDTR + 8 * index, 4);
		descriptor_h = hwaddr_read(cpu.GDTR + 4 + 8 * index, 4);
	}
	else
	{
		descriptor_l = hwaddr_read(cpu.LDTR + 8 * index, 4);
		descriptor_h = hwaddr_read(cpu.LDTR + 4 + 8 * index, 4);
	}
	uint8_t DPL = (descriptor_h >> 13) & 0x03;
	

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


	uint32_t base = ((descriptor_l >> 16) & 0x0000FFFF) + ((descriptor_h << 16) & 0x00FF0000) + (descriptor_h & 0xFF000000);
	uint32_t lnaddr = base + addr;

	return lnaddr;
}

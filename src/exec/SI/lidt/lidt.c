#include "exec/helper.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"


// lidt  %IDTR, m16&m32(linear base address)
uint32_t hwaddr_read(hwaddr_t addr, size_t len);
make_helper(lidt)
{
	// ModR_M:  mod  reg  R_M
	//           00  011  101
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	assert( m.val == 0x1D );

	uint32_t ph_addr = hwaddr_read(eip+2, 4);
	cpu.IDTR.limit = hwaddr_read(ph_addr    , 2);
	cpu.IDTR.base  = hwaddr_read(ph_addr + 2, 4);

	print_asm("lidt   0x%x", ph_addr);

	return 1 + 1 + 4;
}

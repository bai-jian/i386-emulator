#include "exec/helper.h"


#include "cpu/reg.h"

extern char suffix;

make_helper(rep)
{
	uint8_t inst = instr_fetch(eip+1, 1);
	if ( inst == 0xa4 )
	{
		while ( (reg_b(R_CL))-- )
		{
			swaddr_write( reg_l(R_EDI), 1, swaddr_read(reg_l(R_ESI), 1) );
		
			if ( cpu.DF )
				(reg_l(R_ESI))--, (reg_l(R_EDI))--;
		    else
				(reg_l(R_ESI))++, (reg_l(R_EDI))++;	
		}
		print_asm("rep movsb %%ds:(%%esi),%%es:(%%edi)");

		return 2;
	}
	if ( inst == 0xa5 && suffix == 'w' )
	{
		while ( (reg_w(R_CX))-- )
		{
			swaddr_write( reg_l(R_EDI), 2, swaddr_read(reg_l(R_ESI), 2) );

			if ( cpu.DF )
				(reg_l(R_ESI)) -= 2, (reg_l(R_EDI)) -= 2;
			else
				(reg_l(R_ESI)) += 2, (reg_l(R_EDI)) += 2;
		}
		print_asm("rep movsw %%ds:(%%esi),%%es:(%%edi)");

		return 2;	
	}
	if ( inst == 0xa5 && suffix == 'l' )
	{
		while( (reg_l(R_ECX))-- )
		{
			swaddr_write( reg_l(R_EDI), 4, swaddr_read(reg_l(R_ESI), 4) );

			if ( cpu.DF )
				(reg_l(R_ESI)) -= 4, (reg_l(R_EDI)) -= 4;
			else
				(reg_l(R_ESI)) += 4, (reg_l(R_EDI)) += 4;
		}
		print_asm("rep movsl %%ds:(%%esi),%%es:(%%edi)");

		return 2;
	}
	assert(0);  return 0;
}

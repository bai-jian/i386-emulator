#include "exec/helper.h"


#include "cpu/reg.h"

extern char suffix;
extern uint8_t current_sreg;

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

	// REP STOS m8/m16/m32
	if ( inst == 0xaa )
	{
		while ( (reg_b(R_CL))-- )
		{
			current_sreg = R_ES;
			swaddr_write( reg_l(R_EDI), 1, reg_l(R_AL) );

			reg_l(R_EDI) +=  cpu.DF ? -1 : 1;
		}
		print_asm("rep stosb  %%ES:%%edi");

		return 2;
	}
	if ( inst == 0xab && suffix == 'w' )
	{
		while ( (reg_w(R_CX))-- )
		{
			current_sreg = R_ES;
			swaddr_write( reg_l(R_EDI), 2, reg_w(R_AX) );

			reg_l(R_EDI) +=  cpu.DF ? -2 : 2;
		}
		print_asm("rep stosw  %%ES:%%edi");

		return 2;
	}
	if ( inst == 0xab && suffix == 'l' )
	{
		while( (reg_l(R_ECX))-- )
		{
			current_sreg = R_ES;
			swaddr_write( reg_l(R_EDI), 4, reg_l(R_EAX) );

			reg_l(R_EDI) +=  cpu.DF ? -4 : 4;
		}
		print_asm("rep stosl  %%ES:%%edi");

		return 2;
	}

	assert(0);  return 0;
}

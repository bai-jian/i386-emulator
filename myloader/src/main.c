
void loader( );

void init( )
{
	loader();

	nemu_assert(0);
}


#include <elf.h>
#include "memory.h"
void loader( )
{
	Elf32_Ehdr* elf = (void*) 0;

	foreach ph in ph_table
	{ 
		if ( ph->p_type == PT_LOAD )
	 	{
			assert(0);
		}
	}

	(  ( void(*) (void) )  elf->entry) ( );

	HIT_GOOD_TRAP;
}

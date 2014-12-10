#include "trap.h"

#include <elf.h>


#define ELF_START 0

void loader( )
{
	Elf32_Ehdr* elf = (void*) ELF_START;
	Elf32_Phdr* ph = (void*)elf->e_phoff;

	int i;
	for (i = 0; i < elf->e_phnum; ++i)
	{
		if (ph[i].p_type == PT_LOAD)
		{
			int j;
			for (j = 0; j < ph[i].p_filesz; ++j)
				*(uint8_t*)(ph[i].p_vaddr + j) = *(uint8_t*)(ph[i].p_offset + j);
			for (; j < ph[i].p_memsz; ++j)
				*(uint8_t*)(ph[i].p_vaddr + j) = 0;
		}
	}

	( (void(*)(void)) elf->e_entry) ();

//	HIT_GOOD_TRAP;
}


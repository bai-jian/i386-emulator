#include "common.h"
#include "memory.h"
#include <string.h>

#include <elf.h>

#ifdef HAS_DEVICE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

#define ELF_START 0

uint32_t loader( )
{
	Elf32_Ehdr* elf = (void*)ELF_START;
	Elf32_Phdr* ph = (void*)elf->e_phoff;

#ifdef HAS_DEVICE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
#else
	/* The ELF file is located at memory address 0 */
	elf = (void *)0x0;
#endif

	// Load each program segment 
	uint32_t i;
	for (i = 0; i < elf->e_phnum; ++i)
	{
		// Scan the program header table, load each segment into memory
		if (ph[i].p_type == PT_LOAD)
		{
			uint32_t j;

			// read the content of the segment from the ELF file to the memory region [VirtAddr, VirtAddr + FileSiz)
			for (j = 0; j < ph[i].p_filesz; ++j)
				*(uint8_t*)((uint8_t*)ph[i].p_vaddr + j) = *(uint8_t*)((uint8_t*)ph[i].p_offset + j);
			 
			// zero the memory region [VirtAddr + FileSiz, VirtAddr + MemSiz)
			for (;      j < ph[i].p_memsz;  ++j)
				*(uint8_t*)((uint8_t*)ph[i].p_vaddr + j) = 0; 

			/* Record the prgram break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}

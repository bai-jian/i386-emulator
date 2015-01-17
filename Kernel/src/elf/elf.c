#include "common.h"
#include "memory.h"

#include <elf.h>
#include <string.h>


#ifdef HAS_DEVICE
	#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

#define BUF_H_SIZE 4096
#define BUF_T_SIZE 4096
static uint8_t buf_h[BUF_H_SIZE];  // buffer head
static uint8_t buf_t[BUF_T_SIZE];  // buffer temp

void ide_read(uint8_t*, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();
uint32_t loader( )
{
	Elf32_Ehdr* elf;

	#ifdef HAS_DEVICE
		ide_read(buf_h, ELF_OFFSET_IN_DISK, 4096);
		elf = (void*)buf_h;
	#else
		// The ELF file is located at memory address 0
		elf = (void*)0x0;
	#endif


	// The first several bytes contain the ELF header
	// Check ELF header
//	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
//	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	Elf32_Phdr* ph = (void*)elf->e_phoff;
	Log("HI 1 + 1 = %d\n", 2);
	Log("%d\n", elf->e_phnum);
	Log("%d\n", ph[0].p_offset);
	Log("%d\n", ph[0].p_filesz);
	Log("%d\n", ph[0].p_memsz);

	// Load each program segment 
	uint32_t i;
	for (i = 0; i < elf->e_phnum; ++i)
	{
	Log("%d\n", ph[i].p_offset);
	Log("%d\n", ph[i].p_filesz);
	Log("%d\n", ph[i].p_memsz);
		// Scan the program header table, load each segment into memory
		if (ph[i].p_type == PT_LOAD)
		{
			#ifdef IA32_PAGE

				uint32_t pa = mm_malloc(ph[i].p_vaddr, ph[i].p_memsz);

				assert(ph[i].p_filesz < BUF_T_SIZE);
				ide_read(buf_t, ph[i].p_offset, ph[i].p_filesz);

				uint32_t j;
				for (j = 0; j < ph[i].p_filesz; ++j)
					*(uint8_t*)(pa + j) = buf_t[j];
				for (     ; j < ph[i].p_memsz;  ++j)
					*(uint8_t*)(pa + j) = (uint8_t)0;

				int k;
				for (k = 0; k < ph[i].p_filesz; ++k)
					Log("0x%x ", buf_t[k]);
				assert(0);

			#else
				uint32_t j;
				// read the content of the segment from the ELF file to the memory region [VirtAddr, VirtAddr + FileSiz)
				for (j = 0; j < ph[i].p_filesz; ++j)
					*(uint8_t*)((uint8_t*)ph[i].p_vaddr + j) = *(uint8_t*)((uint8_t*)ph[i].p_offset + j);
				// zero the memory region [VirtAddr + FileSiz, VirtAddr + MemSiz)
				for (     ; j < ph[i].p_memsz;  ++j)
					*(uint8_t*)((uint8_t*)ph[i].p_vaddr + j) = 0; 
			#endif

			/* Record the prgram break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
		}
	}

	assert(0);

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

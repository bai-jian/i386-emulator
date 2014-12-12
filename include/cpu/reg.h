#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"


typedef struct
{
	// General Purpose Registers
		union
		{
			union
			{
				uint32_t _32;
				uint16_t _16;
				uint8_t _8[2];
			} gpr[8];
			struct
			{
				uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
			};
		};

	// Flags Register
		union
		{ 
			uint32_t eflags;
			struct
		 	{
				uint32_t CF  :  1;
				uint32_t     :  1;
				uint32_t PF  :  1;
				uint32_t     :  1;
				uint32_t AF  :  1;
				uint32_t     :  1;
				uint32_t ZF  :  1;
				uint32_t SF  :  1;
				uint32_t TF  :  1;
				uint32_t IF  :  1;
				uint32_t DF  :  1;
				uint32_t OF  :  1;
				uint32_t IOPL:  2;
				uint32_t NT  :  1;
				uint32_t     :  1;
				uint32_t RF  :  1;
				uint32_t VM  :  1;
				uint32_t     :  14;
			};
		};

	// Instruction Pointer Register
		uint32_t eip;

		
	// Segment Registers
		union
		{
			uint16_t sreg[6];
			struct
			{
				uint16_t CS, SS, DS, ES, FS, GS;
			};
		};

	// Globl/Local Descriptor Table Registers
		uint32_t GDTR, LDTR;

	// Control Registers
		union
		{
			uint32_t CR[4];
			struct
			{
				struct
				{
					uint32_t CR0_PE	: 1;  // Proctetion Enable
					uint32_t CR0_MP : 1;  // Math Present
					uint32_t CR0_EM : 1;  // EMulation
					uint32_t CR0_TS : 1;  // Task Switched
					uint32_t CR0_ET : 1;  // Extension Type
					uint32_t    : 26; // reserved
					uint32_t CR0_PG : 1;  // paging
				};
				// CR1: reserved
				// CR2: Page Fault Linear Address
				// CR3:
			};
		};
} CPU_state;
extern CPU_state cpu;

// GPR: usual usage
#define reg_l(index) (cpu.gpr[index]._32)
#define reg_w(index) (cpu.gpr[index]._16)
#define reg_b(index) (cpu.gpr[index & 0x3]._8[index >> 2])
enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
// GPR: name strings
extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

// SREG
extern uint8_t current_sreg;
enum { R_CS, R_SS, R_DS, R_ES, R_FS, R_GS };

#endif

#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"

/** GDTR ************************************************************/
/*              +--------------------------------+----------------+ */
/*              |            GDT base            |     limit      | */
/*              +--------------------------------+----------------+ */
/*               47                            16|15             0  */
/********************************************************************/

/** IDTR ************************************************************/
/*              +--------------------------------+----------------+ */
/*              |            IDT base            |     limit      | */
/*              +--------------------------------+----------------+ */
/*               47                            16|15             0  */
/********************************************************************/

/** CR0 *************************************************************/
/*                   +-+-+-+----------+-+-+-+---------+-+-+-+-+-+-+ */
/*                   |P|C|N|          |A| |W|         |N|E|T|E|M|P| */
/*                   |G|D|W|          |M| |P|         |E|T|S|M|C|E| */
/*                   +-+-+-+----------+-+-+-+---------+-+-+-+-+-+-+ */
/*                   31  29 28      19 18 16 15      6 5 4 3 2 1 0  */
/*                                                                  */
/*                    *PG: paging                                   */
/*                     CD: cache disable                            */
/*                     NW: not write through                        */
/*                     AM: alignment mask                           */
/*                     WP: write protect                            */
/*                     NE: numeric error                            */
/*                     ET: extension type                           */
/*                     TS: task switched                            */
/*                     EM: emulation                                */
/*                     MC: monitor coprocessor                      */
/*                    *PE: protect enable                           */
/********************************************************************/

/** CR3 *************************************************************/
/*                   +------------------------+---------+-+-+-----+ */
/*                   |        directory       |         |C|W|     | */
/*                   |          base          |         |D|T|     | */ 
/*                   +------------------------+---------+-+-+-----+ */
/*                    31                    12 11      5 4 3 2   0  */
/*                                                                  */
/*                    CD: page cache disable                        */
/*                    WT: page write through                        */
/********************************************************************/

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
		union
		{
			uint16_t ip;
			uint32_t eip;
		};
		
	// Segment Registers
		union
		{
			union
			{
				uint16_t val;
				struct
				{
					uint16_t RPL	:  2;
					uint16_t TI		:  1;
					uint16_t index	:  13;
				};
			} sreg[6];
			struct
			{
				uint16_t ES, CS, SS, DS, FS, GS;
			};
		};

	// Globl/Local/Interrupt Descriptor Table Registers
		struct
		{
			struct
			{
				uint16_t limit;
				uint32_t base;
			} GDTR;
			struct
			{
				uint16_t limit;
				uint32_t base;
			} LDTR;
			struct
			{
				uint16_t limit;
				uint32_t base;
			} IDTR;
		};

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
				struct
				{
					uint32_t     : 32;
				};
				// CR2: Page Fault Linear Address
				struct
				{
					uint32_t     :  32;
				};
				// CR3: Page Directory Base Address (aligning 4KB)
				struct
				{
					uint32_t     :  12;
					uint32_t CR3_pdba:  20;
				};
			};
		};

	// Intrrupt Register
		bool INTR;
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

// IP
#define ip_w  cpu.ip
#define ip_l  cpu.eip

// SREG
extern uint8_t current_sreg;
enum { R_ES, R_CS, R_SS, R_DS, R_FS, R_GS };
extern const char* sregs[];


#endif

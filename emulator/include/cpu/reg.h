#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"

struct reg_t
{
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

	union
	{ 
		uint32_t eflags;
		struct
		{
			uint32_t CF   : 1;
			uint32_t      : 1;
			uint32_t PF   : 1;
			uint32_t      : 1;
			uint32_t AF   : 1;
			uint32_t      : 1;
			uint32_t ZF   : 1;
			uint32_t SF   : 1;
			uint32_t TF   : 1;
			uint32_t IF   : 1;
			uint32_t DF   : 1;
			uint32_t OF   : 1;
			uint32_t IOPL : 2;
			uint32_t NT   : 1;
			uint32_t      : 1;
			uint32_t RF   : 1;
			uint32_t VM   : 1;
			uint32_t      : 14;
		};
	};

	union
	{
		uint16_t ip;
		uint32_t eip;
	};
		
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

	union
	{
		uint32_t CR[4];
		struct
		{
			struct
			{
				uint32_t CR0_PE	: 1;
				uint32_t CR0_MP : 1;
				uint32_t CR0_EM : 1;
				uint32_t CR0_TS : 1;
				uint32_t CR0_ET : 1;
				uint32_t        : 26;
				uint32_t CR0_PG : 1;
			};
			struct
			{
				uint32_t : 32;
			};
			struct
			{
				uint32_t : 32;
			};
			struct
			{
				uint32_t          : 12;
				uint32_t CR3_pdba : 20;
			};
		};
	};

	bool INTR;
};

struct reg_t cpu;

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

#ifndef __CPU_MODRM_H__
#define __CPU_MODRM_H__


#include "common.h"


typedef union
{
	uint8_t val;
	struct
	{
		uint8_t R_M		:3;
		uint8_t reg		:3;
		uint8_t mod		:2;
 	};
	struct
	{
		uint8_t dont_care	:3;
		uint8_t opcode		:3;
 	};
} ModR_M;

typedef union
{
	uint8_t val;
 	struct
	{
		uint8_t base	:3;
		uint8_t index	:3;
		uint8_t ss		:2;
	};
} SIB;

#define MODRM_ASM_BUF_SIZE 32
extern char ModR_M_asm[];


int read_ModR_M(swaddr_t, swaddr_t*);


#endif

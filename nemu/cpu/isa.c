#include "common.h"

typedef struct _instr_prefix_t
{
	ub_t instr;
	ub_t addr_size;
	ub_t operand_size;
	ub_t segment_override;
} instr_prefix_t;

static instr_prefix_t prefix;

static void prefix_clear()
{
	prefix.instr = 0;
	prefix.addr_size = 0;
	prefix.operand_size = 0;
	prefix.segment_override = 0;
}

/*
static inline void _instr_exec();

static void set_addr_size_prefix()
{
	prefix.addr_size = 0x67;
	_instr_exec();
}

static void set_operand_size_prefix()
{
	prefix.operand_size = 0x66;
	_instr_exec();
}

typedef void (*instr_t)();

instr_t isa[0x100] =
{

};


instr_t isa_twobyte[0x100] =
{

};

static inline void _instr_exec()
{
	ub_t instr;
	// instr_fetch(1, &instr);

	isa[instr]();
}

void instr_exec()
{
	prefix_clear();
	
	_instr_exec();
	
	// instr_echo();
}
*/


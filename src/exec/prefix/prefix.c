#include "exec/helper.h"

#include "nemu.h"

int exec(swaddr_t);

char suffix = 'l';

make_helper(data_size) {
	suffix = 'w';
	int instr_len = exec(eip + 1);
	suffix = 'l';
	return instr_len + 1;
}

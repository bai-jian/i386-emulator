#include "exec/helper.h"

#define  DATA_BYTE 1
#include "opcode-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "opcode-template.h"
#undef	 DATA_BYTE

#define  DATA_BYTE 4
#include "opcode-template.h"
#undef   DATA_BYTE

extern char suffix;

make_helper(inst_v)
{	return ( suffix == 'l' ? inst_l(eip) : inst_w(eip) ); 	}

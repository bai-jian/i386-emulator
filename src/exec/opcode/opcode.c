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

make_helper(IG_FF_v)
{	return ( suffix == 'l' ? IG_FF_l(eip) : IG_FF_w(eip) );		}

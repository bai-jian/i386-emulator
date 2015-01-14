#include "exec/helper.h"


#define  DATA_BYTE 2
#include "cmovcc-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "cmovcc-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(cmove_v)
{	return  suffix == 'l' ? cmove_l(eip) : cmove_w(eip);	} 

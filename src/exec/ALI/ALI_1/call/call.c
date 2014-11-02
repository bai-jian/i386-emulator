#include "exec/helper.h"

#define  DATA_BYTE 1
#include "call-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "call-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "call-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(call_i_v)
{	return  suffix == 'l'  ?  call_i_l(eip)  :  call_i_w(eip);	}

make_helper(call_rm_v)
{	return  suffix == 'l'  ?  call_rm_l(eip) :  call_rm_w(eip);	}

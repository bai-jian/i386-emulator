#include "exec/helper.h"

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE


extern char suffix;

make_helper(push_r_v)
{	return ( suffix == 'l' ? push_r_l(eip) : push_r_w(eip) );  }



#include "exec/helper.h"

#define  DATA_BYTE 1
#include "inc-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "inc-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "inc-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(inc_r_v)
{	return  suffix == 'l'  ?  inc_r_l(eip)   :  inc_r_w(eip);	} 

make_helper(inc_rm_v)
{	return  suffix == 'l'  ?  inc_rm_l(eip)  :  inc_rm_w(eip);	}

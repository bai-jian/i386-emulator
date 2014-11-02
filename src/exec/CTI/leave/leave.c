#include "exec/helper.h"

#define  DATA_BYTE 1
#include "leave-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "leave-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "leave-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(leave_v)
{	return  suffix == 'l'  ?  leave_l(eip)  :  leave_w(eip); 	}

#include "exec/helper.h"

#define  DATA_BYTE 1
#include "sal-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "sal-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "sal-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(sal_v)
{	return  suffix == 'l'  ?  sal_l(eip)  :  sal_w(eip);	}

make_helper(sal_CL_v)
{	return  suffix == 'l'  ?  sal_CL_l(eip)  :  sal_CL_w(eip);	}

make_helper(sal_ib_v)
{	return  suffix == 'l'  ?  sal_ib_l(eip)	 :  sal_ib_w(eip);	}

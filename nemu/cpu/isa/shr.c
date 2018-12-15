#include "helper.h"

#define  DATA_BYTE 1
#include "shr-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "shr-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "shr-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(shr_v)
{	return  suffix == 'l'  ?  shr_l(eip)  :  shr_w(eip);	}

make_helper(shr_CL_v)
{	return  suffix == 'l'  ?  shr_CL_l(eip)  :  shr_CL_w(eip);	}

make_helper(shr_ib_v)
{	return  suffix == 'l'  ?  shr_ib_l(eip)  :  shr_ib_w(eip);	}

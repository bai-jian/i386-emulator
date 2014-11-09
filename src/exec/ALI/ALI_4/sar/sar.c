#include "exec/helper.h"

#define  DATA_BYTE 1
#include "sar-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "sar-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "sar-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(sar_v)
{	return  suffix == 'l'  ?  sar_l(eip)  :  sar_w(eip);	}

make_helper(sar_CL_v)
{	return  suffix == 'l'  ?  sar_CL_l(eip)  :  sar_CL_w(eip);	}

make_helper(sar_ib_v)
{	return  suffix == 'l'  ?  sar_ib_l(eip)  :  sar_ib_w(eip);	}

#include "exec/helper.h"

#define  DATA_BYTE 1
#include "and-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "and-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "and-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(and_i2r0_v)
{	return  suffix == 'l'  ?  and_i2r0_l(eip)  :  and_i2r0_w(eip);	}

make_helper(and_i2rm_v)
{	return  suffix == 'l'  ?  and_i2rm_l(eip)  :  and_i2rm_w(eip);	}

make_helper(and_r2rm_v)
{	return  suffix == 'l'  ?  and_r2rm_l(eip)  :  and_r2rm_w(eip);	}

make_helper(and_rm2r_v)
{	return  suffix == 'l'  ?  and_rm2r_l(eip)  :  and_rm2r_w(eip);	}

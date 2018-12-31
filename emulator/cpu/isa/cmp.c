#include "helper.h"

#define  DATA_BYTE 1
#include "cmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "cmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "cmp-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(cmp_i2r0_v)
{	return ( suffix == 'l' ? cmp_i2r0_l(eip) : cmp_i2r0_w(eip) );	}

make_helper(cmp_i2rm_v)
{	return ( suffix == 'l' ? cmp_i2rm_l(eip) : cmp_i2rm_w(eip) ); 	}

make_helper(cmp_r2rm_v)
{	return ( suffix == 'l' ? cmp_r2rm_l(eip) : cmp_i2rm_w(eip) );	}

make_helper(cmp_rm2r_v)
{	return ( suffix == 'l' ? cmp_rm2r_l(eip) : cmp_rm2r_w(eip) );	}

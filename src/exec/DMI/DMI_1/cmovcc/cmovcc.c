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

make_helper(cmova_v)
{	return  suffix == 'l' ? cmova_l(eip) : cmova_w(eip);	}

make_helper(cmovs_v)
{	return  suffix == 'l' ? cmovs_l(eip) : cmovs_w(eip);	}

make_helper(cmovge_v)
{	return  suffix == 'l' ? cmovge_l(eip) : cmovge_w(eip);	}

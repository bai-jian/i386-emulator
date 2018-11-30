#include "exec/helper.h"

#define  DATA_BYTE 2
#include "pop-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "pop-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(pop_r_v)
{	return (suffix == 'l'  ?  pop_r_l(eip)  :  pop_r_w(eip));	}

make_helper(pop_m_v)
{	return (suffix == 'l'  ?  pop_m_l(eip)  :  pop_m_w(eip));	}

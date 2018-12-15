#include "helper.h"

#define  DATA_BYTE 1
#include "push-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "push-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "push-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(push_i_v)
{	return ( suffix == 'l' ? push_i_l(eip) : push_i_w(eip) );	}

make_helper(push_r_v) 
{	return ( suffix == 'l' ? push_r_l(eip) : push_r_w(eip) );	}

make_helper(push_m_v)
{	return ( suffix == 'l' ? push_m_l(eip) : push_m_w(eip) );	}

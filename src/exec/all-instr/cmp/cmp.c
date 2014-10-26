#include "exec/helper.h"

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

make_helper(cmp_i82rm_v)
{	return ( suffix == 'l' ? cmp_i82rm_l(eip) : cmp_i82rm_w(eip) ); }

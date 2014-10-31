#include "exec/helper.h"

#define  DATA_BYTE 1
#include "dec-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "dec-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "dec-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(dec_r_v)
{	return  suffix == 'l'  ?  dec_r_l(eip)  :  dec_r_w(eip);		}

make_helper(dec_rm_v)
{	return  suffix == 'l'  ?  dec_rm_l(eip) :  dec_rm_w(eip);		}

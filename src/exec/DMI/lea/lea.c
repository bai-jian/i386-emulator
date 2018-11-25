#include "exec/helper.h"


#define  DATA_BYTE 2
#include "lea-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "lea-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(lea_m2r_v)
{	return  (suffix == 'l'  ?  lea_m2r_l(eip)  :  lea_m2r_w(eip));	}

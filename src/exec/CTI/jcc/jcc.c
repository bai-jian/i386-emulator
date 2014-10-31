#include "exec/helper.h"

#define  DATA_BYTE 1
#include "jcc-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "jcc-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "jcc-template.h"
#undef   DATA_BYTE


extern char suffix;
/*
make_helper(jbe_v)
{	return  suffix == 'l'  ?  jbe_l(eip)  :  jbe_w(eip);	}
*/

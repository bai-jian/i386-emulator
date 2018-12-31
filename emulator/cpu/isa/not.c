#include "helper.h"

#define  DATA_BYTE 1
#include "not-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "not-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "not-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(not_v)
{	return  suffix == 'l'  ?  not_l(eip)  :  not_w(eip);	}

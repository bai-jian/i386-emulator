#include "exec/helper.h"


#define DATA_BYTE 2
#include "lgdt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "lgdt-template.h"
#undef DATA_BYTE


extern char suffix;

make_helper(lgdt_v)
{	return  suffix == 'l'  ?  lgdt_l(eip)  :  lgdt_w(eip);	}

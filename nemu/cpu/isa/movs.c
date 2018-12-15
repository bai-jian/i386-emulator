#include "helper.h"

#define DATA_BYTE 1
#include "movs-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movs-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movs-template.h"
#undef DATA_BYTE


extern char suffix;

make_helper(movs_v)
{	return  suffix == 'l'  ?  movs_l(eip)  :  movs_w(eip);	}

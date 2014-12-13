#include "exec/helper.h"

#define  DATA_BYTE 1
#include "jmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "jmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "jmp-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(jmp_i_v)
{	return  suffix == 'l'  ?  jmp_i_l(eip)  :  jmp_i_w(eip);  }

make_helper(jmp_rm_v)
{	return  suffix == 'l'  ?  jmp_rm_l(eip)  :  jmp_rm_w(eip);	}

make_helper(jmp_seg_v)
{	return  suffix == 'l'  ?  jmp_seg_l(eip)  :  jmp_seg_w(eip);  } 

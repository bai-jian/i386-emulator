#include "exec/helper.h"


#define  DATA_BYTE 1
#include "imul-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "imul-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "imul-template.h"
#undef   DATA_BYTE


extern char suffix;
/*
make_helper(imul_rm_v)
{	return (suffix == 'l'  ?  imul_rm_l(eip)      :  imul_rm_w(eip));		}
*/
make_helper(imul_rm2r_v)
{	return (suffix == 'l'  ?  imul_rm2r_l(eip)    :  imul_rm2r_w(eip));		}
/*
make_helper(imul_rmib2r_v)
{	return (suffix == 'l'  ?  imul_rmib2r_l(eip)  :  imul_rmib2r_w(eip));	}

make_helper(imul_rmi2r_v)
{	return (suffix == 'l'  ?  imul_rmi2r_l(eip)   :  imul_rmi2r_w(eip));	}
*/

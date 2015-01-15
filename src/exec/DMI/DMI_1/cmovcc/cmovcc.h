#ifndef __CMOVCC__
#define __CMOVCC__

/* 0F 44 */  make_helper(cmove_v);
/* 0F 45 */  make_helper(cmovne_v);
/* 0F 47 */  make_helper(cmova_v);		// make_helper(cmovnbe_v);
/* 0F 48 */  make_helper(cmovs_v);		
/* 0F 49 */  make_helper(cmovns_v);
/* 0F 4D */  make_helper(cmovge_v);		// make_helper(cmovnl_v);

#endif

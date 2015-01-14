#include "exec/helper.h"


#include "cpu/reg.h"


make_helper(cli)  {  cpu.IF = 0;  print_asm("cli");  return 1;  }
make_helper(cld)  {  cpu.DF = 0;  print_asm("cld");  return 1;  }

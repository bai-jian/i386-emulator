#include "helper.h"
#include "cpu/reg.h"

make_helper(clc)  {  cpu.CF = 0;  print_asm("clc");  return 1;  }
make_helper(stc)  {  cpu.CF = 1;  print_asm("stc");  return 1;  }
make_helper(cli)  {  cpu.IF = 0;  print_asm("cli");  return 1;  }
make_helper(sti)  {  cpu.IF = 1;  print_asm("sti");  return 1;  }
make_helper(cld)  {  cpu.DF = 0;  print_asm("cld");  return 1;  }
make_helper(std)  {  cpu.DF = 1;  print_asm("std");  return 1;  }

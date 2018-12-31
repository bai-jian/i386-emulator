#include "common.h"
#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "ui/watchpoint.h"
#include "ui/expr.h"
#include "cpu/reg.h"
#include "cpu/mm.h"
#include <readline/readline.h>
#include <readline/history.h>


// enum { END, STOP, RUNNING, INT }
int nemu_state = END;


// Commond Resolution
static char* line_read = NULL;
static char* saveptr = NULL;
static char* cmdptr = NULL;
void command() 
{
	if (line_read) 
	{
		free(line_read);
		line_read = NULL; 
	}

	if (nemu_state == INT)
		line_read = readline("(nemu/debug) ");
	else
		line_read = readline("(nemu) ");


	if (line_read && *line_read)
		add_history(line_read);

	cmdptr = strtok_r(line_read, " ", &saveptr);
}

// Initialize the nemu
void restart();
// Execute instructions of some length
#define INSTR_LEN  ( (strtol(saveptr, NULL, 0) < 1)  ?  1  :  strtol(saveptr, NULL, 0) )
#define INSTR_END  -1
static uint32_t instr_len;
void cpu_exec(uint32_t);

static void cmd_END_r();
static void cmd_STOP_r();
static void cmd_STOP_c();
static void cmd_STOP_d();
static void cmd_INT_r();
static void cmd_INT_c();
static void cmd_INT_si();
static void cmd_INT_info();
static void cmd_INT_x();
static void cmd_INT_p();
static void cmd_INT_bt();
static void cmd_INT_b();
static void cmd_INT_w();
static void cmd_INT_d();
static void cmd_RUNNING()	{ return cpu_exec(instr_len); }
void main_loop() 
{ 
 	while( true )
 	{ 
		switch( nemu_state )  // NEMU: a State Machine(nemu_state = END when initialization)
		{
			case END:
			{
        		command();
				if (cmdptr == NULL)             {  continue;  }
				if (strcmp(cmdptr, "q")  == 0)  {  return;    }
				if (strcmp(cmdptr, "r")  == 0)	{  cmd_END_r();   nemu_state = RUNNING;  continue;  }
				puts("The Program does not start. Use command 'r' to start the program.");
				break;
			}
			case STOP:
			{
				command();
				if (cmdptr == NULL)            {  continue;  }
				if (strcmp(cmdptr, "q") == 0)  {  return;    }
				if (strcmp(cmdptr, "r") == 0)  {  cmd_STOP_r();   nemu_state = RUNNING;  continue;  }
				if (strcmp(cmdptr, "c") == 0)  {  cmd_STOP_c();   nemu_state = RUNNING;  continue;  }
				if (strcmp(cmdptr, "d") == 0)  {  cmd_STOP_d();   nemu_state = INT;      continue;  }
				printf("Unnow command '%s'.\n", cmdptr);
				break;
			}
			case INT:
			{
				command();
				if (cmdptr == NULL)             {  continue; }
				if (strcmp(cmdptr, "q")  == 0)  {  return;   }
				if (strcmp(cmdptr, "r")  == 0)  {  cmd_INT_r();   nemu_state = RUNNING;  continue;  }
				if (strcmp(cmdptr, "c")  == 0)  {  cmd_INT_c();   nemu_state = RUNNING;  continue;  }
				if (strcmp(cmdptr, "si") == 0)	{  cmd_INT_si();  nemu_state = RUNNING;  continue;  }
				// Look up information of registers, memory, breakpoint, watchpoint, stack frame linked lists
				if (strcmp(cmdptr, "info") == 0)  {  cmd_INT_info();  continue;  } 
				if (strcmp(cmdptr, "x")    == 0)  {  cmd_INT_x();     continue;  }
				if (strcmp(cmdptr, "p")    == 0)  {  cmd_INT_p();     continue;  }
				if (strcmp(cmdptr, "bt")   == 0)  {  cmd_INT_bt();    continue;  }
				// Create or delete breakpoints and watchpoints
				if (strcmp(cmdptr, "b") == 0)  {  cmd_INT_b();  continue;  }
				if (strcmp(cmdptr, "w") == 0)  {  cmd_INT_w();  continue;  }
				if (strcmp(cmdptr, "d") == 0)  {  cmd_INT_d();  continue;  }   
				printf("Unnow command '%s'.\n", cmdptr);
				break;
			}
			case RUNNING:  cmd_RUNNING();
	 	}
    }
}

static void cmd_END_r()  {  restart();  instr_len = INSTR_END;  }

static void cmd_STOP_r()
{
	char c;
	while( true ) 
	{
		printf("The program is running. Restart the program? (y or n)");
		fflush(stdout);
		scanf("%c", &c);
		switch(c)
		{
			case 'y': goto restart_;
			case 'n': return;
			default : puts("Please answer y or n.");
	 	}
	}  
restart_:  cmd_END_r();
}
static void cmd_STOP_c() {  instr_len = INSTR_END;  }

void init_regex();
void init_bp_pool();
void init_wp_pool();
void init_elf_table();
static void cmd_STOP_d()
{
	init_regex();
	init_bp_pool();
	init_wp_pool();
	init_elf_table();
	puts("Debug the user program.");
}

static void cmd_INT_r()  {  cmd_STOP_r();           }
static void cmd_INT_c()  {  instr_len = INSTR_END;  }
static void cmd_INT_si() {  instr_len = INSTR_LEN;  }

static void cmd_INT_info()
{
	if (*saveptr == '\0')
	{
		printf("  Unknown command 'info'. Enter 'info r' or 'info b'.\n");
		return;
	}
	char* p = strtok_r(NULL, " ", &saveptr);

  	if (strcmp(p, "r") == 0) 
	{
		printf("  eax   0x%.8X    %d\n", cpu.eax, cpu.eax);
		printf("  ecx   0x%.8X    %d\n", cpu.ecx, cpu.ecx);
		printf("  edx   0x%.8X    %d\n", cpu.edx, cpu.edx);
		printf("  ebx   0x%.8X    %d\n", cpu.ebx, cpu.ebx);
		printf("  ebp   0x%.8X    %d\n", cpu.ebp, cpu.ebp);
		printf("  esp   0x%.8X    %d\n", cpu.esp, cpu.esp);
		printf("  esi   0x%.8X    %d\n", cpu.esi, cpu.esi);
 		printf("  edi   0x%.8X    %d\n", cpu.edi, cpu.edi);
		printf("  eip   0x%.8X    %d\n", cpu.eip, cpu.eip);
		printf("  CF=%d  ZF=%d  SF=%d  PF=%d  OF=%d\n", (int)cpu.CF, (int)cpu.ZF, (int)cpu.SF, (int)cpu.PF, (int)cpu.OF );
		return;
	}

	if (strcmp(p, "b") == 0)
	{
		print_bp();
		return;
	}

	printf("Unknown command 'info %s'\n", line_read);
}
static void cmd_INT_x()
{
	if (*saveptr == '\0')
	{
		printf("  Unknown command. The formula should be 'x N EXPR'.\n");
		return;
	}	
	char* p1 = strtok_r(NULL, " ", &saveptr);
	if (*saveptr == '\0')
	{
		printf("  Unknown command. The formula should be 'x N EXPR'.\n");
		return;
	}	
	char* p2 = strtok_r(NULL, " ", &saveptr);

	int num = strtol(p1, NULL, 0);
	swaddr_t addr = expr(p2);

	printf("  The beginning memory address: 0x%.8X\n", addr);
	int i;
	for (i = 0; i < num; ++i)
	{
		printf("      0x%.8X", addr + 4*i );
		printf("    0x%.2X",   swaddr_read(addr+4*i+0, 1) );
		printf("    0x%.2X",   swaddr_read(addr+4*i+1, 1) );
		printf("    0x%.2X",   swaddr_read(addr+4*i+2, 1) );
		printf("    0x%.2X\n", swaddr_read(addr+4*i+3, 1) );
	}
}
static void cmd_INT_p()
{
	char* e = strtok_r(NULL, "", &saveptr);
	size_t val = expr(e);

	printf("%s = %u, 0x%x\n", e, val, val);
}

extern char funcname[];
void find_funcname(swaddr_t addr);
static void cmd_INT_bt()
{
	// Initialization
	funcname[0] = '\0';

	swaddr_t ebp = cpu.ebp;
	swaddr_t addr = cpu.eip;
	int i;
	for (i = 0; ebp != 0; ++ i)
	{
		find_funcname(addr);
		printf("    #%d    0x%.8x    %s\n", i, addr, funcname);

		addr = swaddr_read(ebp + 4, 4);
		ebp = swaddr_read(ebp, 4);
	}
}

static void cmd_INT_b()
{
	char* e = strtok_r(NULL, "", &saveptr);
	swaddr_t addr = expr(e);

	new_bp(addr);
}
static void cmd_INT_d()
{
	char* p = strtok_r(NULL, "", &saveptr);
	int n = strtol(p, NULL, 0);

	free_bp(n);
}
static void cmd_INT_w()
{
	char* e = strtok_r(NULL, "", &saveptr);
	new_wp(e);
}


// This function will be called when you press <C-c>. And it will return to where you press <C-c>. 
static void control_C(int signum) 
{
	if(nemu_state == RUNNING) 
		nemu_state = INT;
}
void init_signal() 
{
	// Register a signal handler
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

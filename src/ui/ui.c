#include "ui/ui.h"

#include "nemu.h"
#include "ui/breakpoint.h"
#include "ui/watchpoint.h"
#include "ui/expr.h"


#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


extern uint32_t hit, miss;
extern uint32_t cache_L2_hit, cache_L2, miss;

int nemu_state = END;


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


// Commond Resolution
static char* line_read = NULL;
static char* saveptr = NULL;

// Execute instructions of some length
#define INSTR_LEN  ( (strtol(saveptr, NULL, 0) < 1)  ?  1  :  strtol(saveptr, NULL, 0) )
#define INSTR_END  -1
void cpu_exec(uint32_t);
static void cmd_exec(uint32_t num)  {  cpu_exec(num);  }

char* rl_gets();
void restart();
static void cmd_info();
static void cmd_x();
static void cmd_b();
static void cmd_d();
static void cmd_w();
static void cmd_p();
void main_loop() 
{ 
 	while(1)
 	{ 
        rl_gets();
		char* p = strtok_r(line_read, " ", &saveptr);

		if (p == NULL)            { continue; }
		if (strcmp(p, "q") == 0)  { return;   }


		switch( nemu_state )  // NEMU: a State Machine(nemu_state = END when initialization)
		{
			case END:
				if (strcmp(p, "r")  == 0)	{ nemu_state = RUNNING;  restart();  cmd_exec(INSTR_END);   continue;  }
				if (strcmp(p, "si") == 0)	{ nemu_state = RUNNING;  restart();  cmd_exec(INSTR_LEN);   continue;  }

				puts("The Program does not start. Use 'r' or 'si' command to start the program.");

				break;

			case STOP:
				if (strcmp(p, "r")  == 0)   { nemu_state = RUNNING;  cmd_exec(INSTR_END);  continue;  }
				if (strcmp(p, "c")  == 0)	{ nemu_state = RUNNING;  cmd_exec(INSTR_END);  continue;  }
				if (strcmp(p, "si") == 0)	{ nemu_state = RUNNING;  cmd_exec(INSTR_LEN);  continue;  }

				// Look up information of registers, memory, breakpoint, watchpoint
				if (strcmp(p, "info") == 0) { cmd_info();  continue;  } 
				if (strcmp(p, "x") == 0)	{ cmd_x();	   continue;  }
				if (strcmp(p, "p") == 0)	{ cmd_p();     continue;  }

				// Create or delete breakpoints and watchpoints
				if (strcmp(p, "b") == 0)	{ cmd_b();     continue;  }
				if (strcmp(p, "w") == 0)	{ cmd_w();     continue;  }
				if (strcmp(p, "d") == 0)	{ cmd_d();     continue;  }

				printf("Unknown command '%s'\n", p); 

				break;

			case RUNNING:  break;
			case INT:      break;
	 	}
    }
}

char* rl_gets() 
{
	if (line_read) 
	{
		free(line_read);
		line_read = NULL; 
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read)
		add_history(line_read);

	return line_read;
}

static void cmd_info()
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

static void cmd_x()
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

static void cmd_b()
{
	char* e = strtok_r(NULL, "", &saveptr);
	size_t addr = expr(e);

	new_bp(addr);
}

static void cmd_d()
{
	char* p = strtok_r(NULL, "", &saveptr);
	int n = strtol(p, NULL, 0);

	free_bp(n);
}

static void cmd_w()
{
	char* e = strtok_r(NULL, "", &saveptr);
	new_wp(e);
}
static void cmd_p()
{
	char* e = strtok_r(NULL, "", &saveptr);
	size_t val = expr(e);

	printf("%s = %u\n", e, val);
}

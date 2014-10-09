#include "ui/ui.h"
#include "ui/breakpoint.h"

#include "nemu.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


static char* line_read = NULL;
static char* saveptr = NULL;

int nemu_state = END;

void cpu_exec(uint32_t);
void restart();

/* We use the readline library to provide more flexibility to read from stdin. */

char* rl_gets() {

	if (line_read) {
		free(line_read);
		line_read = NULL; }

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(nemu_state == RUNNING) {
		nemu_state = INT;
	}
}

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

static void cmd_c() {
	if(nemu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return; } 

	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_r() {
	if(nemu_state != END) 
	{
		char c;
		while(1) 
		{
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf(" %c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n."); }
		}
	} 

restart_:
	restart();
	nemu_state = STOP;
	cmd_c();
}

static void cmd_si()
{	cpu.eip = 0x100000; nemu_state = RUNNING;
	char* p = strtok_r(NULL, " ", &saveptr);
	int num = p ? strtol(p, NULL, 0) : 1;

	cpu_exec(num); 

	return; 
}

static void cmd_info()
{
	char* p = strtok_r(NULL, " ", &saveptr);

  	if (strcmp(p, "r") == 0) {
		printf("eax = 0x%8X    %d\n", cpu.eax, cpu.eax);
		printf("ecx = 0x%8X    %d\n", cpu.ecx, cpu.ecx);
		printf("edx = 0x%8X    %d\n", cpu.edx, cpu.edx);
		printf("ebx = 0x%8X    %d\n", cpu.ebx, cpu.ebx);
		printf("ebp = 0x%8X\n", cpu.ebp);
		printf("esp = 0x%8X\n", cpu.esp);
		printf("esi = 0x%8X    %d\n", cpu.esi, cpu.esi);
 		printf("edi = 0x%8X    %d\n", cpu.edi, cpu.edi); }
	else  if (strcmp(p, "b") == 0) { print_bp(); }
	else 
		printf("Unknown command '%s'\n", line_read);

	return;
}
static void cmd_x()
{
	char* p1 = strtok_r(NULL, " ", &saveptr);
	char* p2 = strtok_r(NULL, " ", &saveptr);

	int num = strtol(p1, NULL, 0);
	unsigned int addr = strtol(p2, NULL, 16);

	int i = 0;
	for (i = 0; i < num; ++i) 
		printf("%2X\t %2X\t %2X\t %2X\n", \
				swaddr_read(addr+4*i+0, 1), swaddr_read(addr+4*i+1, 1),\
				swaddr_read(addr+4*i+2, 1), swaddr_read(addr+4*i+3, 1));

	return;
}
static void cmd_b()
{
	char* p = strtok_r(NULL, " ", &saveptr);
	swaddr_t addr = strtol(p+1, NULL, 16);

	new_bp(addr);

	return;
}

static void cmd_d()
{
	char* p = strtok_r(NULL, " ", &saveptr);
	if (p == NULL) printf("fda\n");
	int n = strtol(p, NULL, 0);

	free_bp(n);

	return ;
}

void main_loop() 
{ 
 	while(1)
	{
        rl_gets();
		char* p = strtok_r(line_read, " ", &saveptr);

		if (p == NULL) { continue; }

		if (strcmp(p, "c") == 0) { cmd_c(); }
		else if (strcmp(p, "r") == 0) { cmd_r(); }
		else if (strcmp(p, "q") == 0) { return; }
		else if (strcmp(p, "si") == 0) { cmd_si(); }
		else if (strcmp(p, "info") == 0) { cmd_info(); }
		else if (strcmp(p, "x") == 0) { cmd_x(); }
		else if (strcmp(p, "b") == 0) { cmd_b(); }
		else if (strcmp(p, "d") == 0) { cmd_d(); }
		else { printf("Unknown command '%s'\n", p); }
   	} 

	return;
}

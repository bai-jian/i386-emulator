#include "ui/ui.h"

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
		line_read = NULL;
	}

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
		return;
	}

	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_r() {
	if(nemu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf(" %c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
			}
		}
	}

restart_:
	restart();
	nemu_state = STOP;
	cmd_c();
}

static void cmd_si()
{
	char* p = strtok_r(NULL, " ", &saveptr);

	long num = p ? 1 : strtol(p, NULL, 0);

	cpu_exec(num);

	return;
}

static void cmd_info()
{
	char* p = strtok_r(NULL, " ", &saveptr);

	if (strcmp(p, "r") == 0)
		printf("eax = 0x%8X\tecx = 0x%8X\tedx = 0x%8X\tebx = 0x%8X\n\
				ebp = 0x%8X\tesp = 0x%8X\tesi = 0x%8X\tedi = 0x%8X\n",\
				cpu.eax, cpu.ecx, cpu.edx, cpu.ebx,\
				cpu.ebp, cpu.esp, cpu.esi, cpu.edi);
	else 
		printf("Unknown command '%s'\n", line_read);

	return;
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

		/* TODO: Add more commands */

		else { printf("Unknown command '%s'\n", p); }
 	} 

	return;
}

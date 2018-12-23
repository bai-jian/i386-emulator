#include "common.h"
#include "device/terminal.h"

int enable_debug = false;
int quiet = false;

void init_signal();
void reg_test();
void main_loop();
void set_main_args(int, char * []);

static void process_args(int argc, char* argv[])
{
	char opt;
	while( (opt = getopt(argc, argv, "dq")) != -1)
	{
		switch(opt)
		{
			case 'd':
				enable_debug = true;
				break;
			case 'q':
				quiet = true;
				break;
			default :  test(0, "bad option = %s\n", optarg);  break;
		}
	}

	test(argc > optind, "Program is not given\n");

	set_main_args(argc - optind, argv + optind);
}

int main(int argc, char* argv[])
{
	process_args(argc, argv);

	init_signal();
	
	terminal_init();

	reg_test(); 

	main_loop();

	return 0;
}


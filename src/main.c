#include "common.h"

#include <unistd.h>


/* arguments for main() function */
int enable_debug = false;  // allowing to debug
int quiet = false;         // not allowing to print the execution process

void set_main_args(int, char * []);
static void process_args(int argc, char* argv[])
{
	int opt;
	while( (opt = getopt(argc, argv, "dq")) != -1)
	{
		switch(opt)
		{
			case 'd':  enable_debug = true;                   break;
			case 'q':  quiet = true;                          break;
			default :  test(0, "bad option = %s\n", optarg);  break;
		}
	}

	test(argc > optind, "Program is not given\n");

	set_main_args(argc - optind, argv + optind);
}


void init_bp_pool();
void init_wp_pool();
void init_regex();
void init_signal();
void load_table();
void reg_test();
void main_loop();
int main(int argc, char* argv[])
{
	process_args(argc, argv);

	// Perform some global initialization
	init_regex();
	init_signal();
	init_bp_pool();
	init_wp_pool();
	load_table();


	reg_test();  // Test whether the 'CPU_state' structure is organized correctly.


	main_loop();

	return 0;
}

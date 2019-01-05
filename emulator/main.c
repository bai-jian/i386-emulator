#include "cpu/reg.h"
#include "ram/ram.h"
#include "device/terminal.h"

bool emulator_enable_debug = false;
bool emulator_enable_selftest = false;
int emulator_cache_level = 2;

void init_signal();
void main_loop();
void set_main_args(int, char * []);

void usage()
{
}

int main(int argc, char* argv[])
{
	int option_index = 0;
	static struct option long_options[] =
	{
		{"cache",    required_argument, 0, 'c'},
		{"selftest", no_argument,       0, 's'},
		{0,          0,                 0, 0}
	};

	char opt;
	while ((opt = getopt_long(argc, argv, "dq", long_options, &option_index)) != -1)
	{
		switch(opt)
		{
			case 'c':
			{
				emulator_cache_level = atoi(optarg);
				if (emulator_cache_level == -1)
				{
					LOG("disable dram buffer");
				}
				else if (emulator_cache_level <= 0)
				{
					emulator_cache_level = 0;
					LOG("disable cache");
				}
				else if (emulator_cache_level == 1)
				{
					LOG("enable one level of cache");
				}
				else
				{
					emulator_cache_level = 2;
					LOG("enable two levels of cache");
				}
				break;
			}
			case 'd':
			{
				emulator_enable_debug = true;
				break;
			}
			case 'q':
			{
				emulator_enable_debug = false;
				break;
			}
			case 's':
			{
				emulator_enable_selftest = true;
				break;
			}
			default:
			{
				usage();
				break;
			}
		}
	}

	test(argc > optind, "Program is not given\n");
	set_main_args(argc - optind, argv + optind);

	if (emulator_enable_selftest)
	{
		test_reg();
		test_ram();
	}

	init_signal();
	terminal_init();
	main_loop();

	return 0;
}


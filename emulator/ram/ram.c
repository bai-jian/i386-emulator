#include "ram/ram.h"

#define CACHE_BLOCK_SIZE (1 << 6)
#define CACHE_BLOCK_MASK (CACHE_BLOCK_SIZE - 1)

void ram_read(phyaddr_t addr, size_t len, uint8_t *data)
{
	test(addr < PHY_MEM_SIZE, "addr 0x%x is invalid\n", addr);
	test(len == 1 || len == 2 || len == 4, "len %d is invalid\n", len);

	if ((addr ^ (addr + len - 1)) & ~CACHE_BLOCK_MASK)
	{
		int i;
		for (i = 0; i < len; i++)
			ram_read(addr + i, 1, data + i);
	}
	else
	{
		if (emulator_cache_level >= 2)
			cache_read(addr, len, data);
		else if (emulator_cache_level == 1)
			l2cache_read(addr, len, data);
		else if (emulator_cache_level == 0)
			dram_read(addr, len, data);
		else
			memcpy(data, phy_mem + addr, len);
	}
}

void ram_write(phyaddr_t addr, size_t len, uint8_t *data)
{
	test(addr < PHY_MEM_SIZE, "addr 0x%x is invalid\n", addr);
	test(len == 1 || len == 2 || len == 4, "len %d is invalid\n", len);

	if ((addr ^ (addr + len - 1)) & ~CACHE_BLOCK_MASK)
	{
		int i;
		for (i = 0; i < len; i++)
			ram_write(addr + i, 1, data + i);
	}
	else
	{
		if (emulator_cache_level >= 2)
			cache_write(addr, len, data);
		else if (emulator_cache_level == 1)
			l2cache_write(addr, len, data);
		else if (emulator_cache_level == 0)
			dram_write(addr, len, data);
		else
			memcpy(phy_mem + addr, data, len);
	}
}

void test_ram()
{
	srand(time(0));

	flush_cache();
	flush_l2cache();
	flush_dram();

	int i;
	for (i = 0; i < 10000000; i++)
	{
		phyaddr_t addr = (PHY_MEM_SIZE - 4) * 1.0 * rand() / RAND_MAX;

		uint8_t data[4], data_read[4];
		data[0] = rand();
		data[1] = rand();
		data[2] = rand();
		data[3] = rand();

		ram_write(addr, 1, data);
		ram_read(addr, 1, data_read);
		test(data[0] == data_read[0], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr, data[0], data_read[0]);

		ram_write(addr, 2, data);
		ram_read(addr, 2, data_read);
		test(data[0] == data_read[0], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr, data[0], data_read[0]);
		test(data[1] == data_read[1], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr + 1, data[1], data_read[1]);

		ram_write(addr, 4, data);
		ram_read(addr, 4, data_read);
		test(data[0] == data_read[0], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr, data[0], data_read[0]);
		test(data[1] == data_read[1], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr + 1, data[1], data_read[1]);
		test(data[2] == data_read[2], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr + 2, data[2], data_read[2]);
		test(data[3] == data_read[3], "ram test failed in round %d! [0x%08x]: %02x - %02x\n", i + 1, addr + 3, data[3], data_read[3]);
	}
}


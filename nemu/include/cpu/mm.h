#ifndef __CPU_MM_H__
#define __CPU_MM_H__

#include "common.h"
#include "cpu/reg.h"

struct segment_selector_t
{
	uint16_t privilege_level : 2;
	uint16_t table_indicator : 1;
	uint16_t index           : 13;
};

struct segment_descriptor_t
{
	uint32_t limit_15_0      : 16;
	uint32_t base_15_0       : 16;
	uint32_t base_23_16      : 8;
	uint32_t type            : 4;
	uint32_t segment_type    : 1;
	uint32_t privilege_level : 2;
	uint32_t present         : 1;
	uint32_t limit_19_16     : 4;
	uint32_t                 : 3;
	uint32_t granularity     : 1;
	uint32_t base_31_24      : 8;
};

struct page_table_entry_t
{
	uint32_t present         : 1;
	uint32_t read_write      : 1; 
	uint32_t user_supervisor : 1;
	uint32_t                 : 2;
	uint32_t accessed        : 1;
	uint32_t dirty           : 1;
	uint32_t                 : 5;
	uint32_t page_frame_addr : 20;
};

struct translation_lookaside_buffer_t
{
	uint32_t valid           : 1;
	uint32_t                 : 7;
	uint32_t tag             : 20;
	uint32_t page_frame_addr : 20;
};

void tlb_flush();

void mem_read(swaddr_t addr, size_t len, uint8_t *data);
void mem_write(swaddr_t addr, size_t len, uint8_t *data);

uint32_t swaddr_read(swaddr_t addr, size_t len);
void swaddr_write(swaddr_t addr, size_t len, uint32_t data);
uint32_t instr_fetch(swaddr_t addr, size_t len);

#endif

TARGET := nemu
CFLAGS := -MD -Wall -Wno-unused -Wno-unused-result -fno-strict-aliasing -O2 -Iinclude

cpu/isa.o : $(patsubst %.c,%.o,$(wildcard cpu/isa/*.c))
	$(LD) -r -o $@ $^

OBJS := main.o \
        cpu/cpu.o cpu/isa.o cpu/reg.o cpu/io.o cpu/int.o \
        memory/memory.o memory/seg_tran.o memory/page_tran.o memory/ph_memory.o \
        memory/cache.o memory/cache_L2.o memory/dram.o \
        device/terminal.o device/serial.o device/ide.o device/timer.o device/keyboard.o device/vga.o \
        loader/loader.o loader/elf.o \
        debugger/ui.o debugger/breakpoint.o debugger/expr.o debugger/watchpoint.o

-include $(OBJS:.o=.d)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lreadline -lSDL


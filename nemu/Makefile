TARGET := nemu
CFLAGS := -MD -Wall -Wno-unused -fno-strict-aliasing -O2 -Iinclude

cpu/isa.o : $(patsubst %.c,%.o,$(wildcard cpu/isa/*.c))
	$(LD) -r -o $@ $^

OBJS := main.o \
        cpu/cpu.o cpu/isa.o cpu/reg.o io/port-io.o io/mmio.o \
        memory/memory.o memory/seg_tran.o memory/page_tran.o memory/ph_memory.o \
        memory/cache.o memory/cache_L2.o memory/dram.o \
        device/device.o device/sdl.o device/i8259.o device/ide.o device/keyboard.o \
        device/serial.o device/timer.o device/vga.o device/vga-palette.o \
        loader/loader.o loader/elf.o \
        debugger/ui.o debugger/breakpoint.o debugger/expr.o debugger/watchpoint.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lreadline -lSDL


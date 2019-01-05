TARGET := emulator
CFLAGS := -g -MD -Wall -Wno-unused -Wno-unused-result -Wno-incompatible-pointer-types -fno-strict-aliasing -O0 -Iinclude

cpu/isa.o : $(patsubst %.c,%.o,$(wildcard cpu/isa/*.c))
	$(LD) -r -o $@ $^

OBJS := main.o \
        cpu/cpu.o cpu/isa.o cpu/reg.o cpu/mm.o cpu/io.o cpu/int.o \
        ram/ram.o ram/cache.o ram/l2cache.o ram/dram.o \
        device/terminal.o device/serial.o device/ide.o device/timer.o device/keyboard.o device/vga.o \
        loader/loader.o loader/elf.o \
        debugger/ui.o debugger/breakpoint.o debugger/expr.o debugger/watchpoint.o

-include $(OBJS:.o=.d)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lreadline -lSDL


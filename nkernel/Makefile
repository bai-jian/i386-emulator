TARGET := loader
CFLAGS = -m32 -MD -O2 -Wall -fno-builtin -fno-omit-frame-pointer -fno-stack-protector \
		-Iinclude -Iinclude/newlib
ASFLAGS = -m32 -MD -I./include
LDFLAGS = -Ttext=0xC0100000 -m elf_i386 

CSRC = $(shell find . -name "*.c")
SSRC = $(shell find . -name "*.S")
OBJS = start/start.o
OBJS += $(SSRC:.S=.o) $(CSRC:.c=.o)
OBJS += memory/mm_malloc.o 


all: $(OBJS)
	$(LD) $(LDFLAGS) -e start -o loader $(OBJS) libc.a


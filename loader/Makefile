TARGET := loader
CFLAGS = -m32 -MD -O2 -Wall -Wno-unused -fno-builtin -fno-omit-frame-pointer -fno-stack-protector \
		-Iinclude -I$(LIBC_HEADER)
ASFLAGS = -march=i386 -m32 -MD -Iinclude
LDFLAGS = -Ttext=0xC0100000 -melf_i386

OBJS := start/start.o \
	irq/do_irq.o
CSRC = $(shell find . -name "*.c")
OBJS += $(CSRC:.c=.o)
OBJS += memory/mm_malloc.o 


all: $(OBJS)
	$(LD) $(LDFLAGS) -e start -o loader $(OBJ_START) $(OBJS) $(LIBC_OBJECT)


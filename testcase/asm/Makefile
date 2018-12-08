CC      = gcc
LD      = ld
CFLAGS  = -m32 -Wall -Werror -static -I$(NEMU_HEADER)
ASFLAGS = -m32 -static -I$(NEMU_HEADER)
LDFLAGS = -melf_i386 -Ttext-segment=0x800000

SRCS = $(shell ls *.S)
OBJS = $(SRCS:.S=)

all: $(OBJS)

$(OBJS): % : %.o
	$(LD) $(LDFLAGS) -o $@ $<

clean:
	rm $(OBJS) $(SRCS:.S=.o)

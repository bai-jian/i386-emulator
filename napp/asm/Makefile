CC      = gcc
LD      = ld
CFLAGS  = -m32 -Wall -Werror -static -I..
ASFLAGS = -m32 -static -I..
LDFLAGS = -melf_i386 -Ttext-segment=0x800000

SRCS = $(shell ls *.S)
OBJS = $(SRCS:.S=)

all: $(OBJS)

$(OBJS): % : %.o
	$(LD) $(LDFLAGS) -o $@ $<

clean:
	-rm $(OBJS) $(SRCS:.S=.o) $(SRCS:.S=.txt) 2> /dev/null

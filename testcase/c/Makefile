CC      = gcc
LD		= ld
CFLAGS  = -m32 -Wall -Werror -static -I..
LDFLAGS = -melf_i386 -e main -T ../link.lds

SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=)

all: $(OBJS)

$(OBJS): % : %.o
	$(LD) $(LDFLAGS) -o $@ $<
	objdump -d $@ > $@.txt

clean:
	-rm $(OBJS) $(SRCS:.c=.o) $(SRCS:.c=.txt) 2> /dev/null

CC = gcc
CFLAGS = -m32 -MD -O2 -Wall -Werror -I./include -I../lib/newlib \
		 -fno-builtin -fno-omit-frame-pointer -fno-stack-protector 
LDFLAGS = -melf_i386

GAME = typing
#GAME = nemu-pal

CFLAGS += -I./src/$(GAME)/include

COMMON_FILES = $(shell find src/common -name "*.c")
GAME_FILES = $(shell find src/$(GAME) -name "*.c")

CFILES = $(COMMON_FILES) $(GAME_FILES)
OBJS = $(CFILES:.c=.o) 

game: $(OBJS)
	ld $(LDFLAGS) -e game_init -o game $(OBJS) ../lib/libc.a

-include $(OBJS:.o=.d)

.PHONY: clean pal

pal:game
	dd if=/dev/null of=game seek=2048
	cat `find src/nemu-pal/data -type f | sort` >> game

clean:
	-rm -f game $(OBJS) $(OBJS:.o=.d)

TARGET := typing

CFLAGS  := -m32 -MD -O2 -Wall -Werror -fno-builtin -fno-omit-frame-pointer -fno-stack-protector -Iinclude -I$(LIBC_HEADER)
LDFLAGS := -melf_i386

OBJS := main.o game.o draw.o effect.o keyboard.o \
        device/font.o device/palette.o device/timer.o device/video.o \
        lib/syscall.o lib/FLOAT.o

all: $(OBJS)
	$(LD) $(LDFLAGS) -e game_init -o $(TARGET) $^ $(LIBC_OBJECT)


TARGET := nemu
CFLAGS := -MD -Wall -Wno-unused -fno-strict-aliasing -O2 -Iinclude

SRCS   := $(shell find . -name "*.c")
OBJS   := $(SRCS:.c=.o)


all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lreadline -lSDL


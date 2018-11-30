TARGET := nemu
CFLAGS := -MD -Wall -fno-strict-aliasing -O2 -Iinclude

SRCS   := $(shell find . -name "*.c")
OBJS   := $(SRCS:.c=.o)


all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lreadline -lSDL


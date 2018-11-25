# setting compiler and compile options
CC      = gcc
LD      = ld
# CFLAGS  = -ggdb -MD -Wall -Werror -fno-strict-aliasing -I./include -O2
CFLAGS  = -ggdb -MD -Wall           -fno-strict-aliasing -I./include -O2

# target to compile
CFILES  = $(shell find src/ -name "*.c")
OBJS    = $(CFILES:.c=.o)

# test files
TESTFILE = testcase/game/game
C_TEST_FILE_LIST = $(shell find testcase/c/ -name "*.c")
S_TEST_FILE_LIST = $(shell find testcase/asm/ -name "*.S")
TEST_FILE_LIST = $(C_TEST_FILE_LIST:.c=) $(S_TEST_FILE_LIST:.S=)

nemu: $(OBJS)
	$(CC) -o nemu $(OBJS) $(CFLAGS) -lreadline -lSDL
#	-@git add -A --ignore-errors # KEEP IT
#	-@while (test -e .git/index.lock); do sleep 0.1; done # KEEP IT
#	-@(echo "> compile" && uname -a && uptime && pstree -A) | git commit -F - $(GITFLAGS) # KEEP IT

$(TEST_FILE_LIST):
	cd `dirname $@` && make

LOADER_DIR = Kernel
loader:
	cd $(LOADER_DIR) && make
	objcopy -S -O binary $(LOADER_DIR)/loader loader
	xxd -i loader > src/elf/loader.c
	rm loader

run: nemu
	./nemu  $(TESTFILE) # 2>&1 | tee log.txt

gdb: nemu
	gdb --args ./nemu -dq $(TESTFILE)

test: nemu $(TEST_FILE_LIST)
	bash test.sh $(TEST_FILE_LIST)

-include $(OBJS:.o=.d)

clean:
	-@rm -f nemu $(OBJS) $(OBJS:.o=.d) log.txt 2> /dev/null

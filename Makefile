#**** GNU make manual: http://www.gnu.org/software/make/manual/make.html ****
# If you have questions about the follwing Makefile script, 
# please search the Internet, or RTFM.

# setting compiler and compile options
CC      = gcc
LD      = ld
CFLAGS  = -ggdb -MD -Wall -Werror -fno-strict-aliasing -I./include -O2

# jyy always knows what you have done (*^__^*)
GITFLAGS = -q --author='jyy <njujiangyy@gmail.com>' --no-verify --allow-empty

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
	-@git add -A --ignore-errors # KEEP IT
	-@while (test -e .git/index.lock); do sleep 0.1; done # KEEP IT
	-@(echo "> compile" && uname -a && uptime && pstree -A) | git commit -F - $(GITFLAGS) # KEEP IT

$(TEST_FILE_LIST):
	cd `dirname $@` && make

LOADER_DIR = Kernel
loader:
	cd $(LOADER_DIR) && make
	objcopy -S -O binary $(LOADER_DIR)/loader loader
	xxd -i loader > src/elf/loader.c
	rm loader

run: nemu
	./nemu  $(TESTFILE) 2>&1 | tee log.txt

gdb: nemu
	gdb --args ./nemu -dq $(TESTFILE)

test: nemu $(TEST_FILE_LIST)
	bash test.sh $(TEST_FILE_LIST)


STU_ID=131220095
SHARED_FOLDER=/mnt

submit: clean
	cd testcase && make clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2
	mv ../$(STU_ID).tar.bz2 $(SHARED_FOLDER) || rm ../$(STU_ID).tar.bz2

-include $(OBJS:.o=.d)

clean:
	-rm -f nemu $(OBJS) $(OBJS:.o=.d) log.txt 2> /dev/null

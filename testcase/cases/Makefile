CFLAGS  := -m32 -Wall -Werror -fno-stack-protector -static -I$(LIBC_HEADER)
ASFLAGS := -m32 -static -I$(LIBC_HEADER)
LDFLAGS := -melf_i386 -e main

OBJS := mov \
        add \
        add-longlong \
        bubble-sort \
        fact \
        fib \
        gotbaha \
        if-else \
        leap-year \
        matrix-mul \
        max \
        min3 \
        mov-c \
        pascal \
        prime \
        quick-sort \
        select-sort \
        shuixianhua \
        struct \
        sub-longlong \
        sum \
        switch \
        to-lower-case \
        wanshu \
        hello-asm

hello: hello.o
	$(LD) $(LDFLAGS) -o $@.out $< $(LIBC_OBJECT)

$(OBJS): % : %.o
	$(LD) $(LDFLAGS) -o $@.out $<

all: $(OBJS) hello


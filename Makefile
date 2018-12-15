CC := gcc
LD := ld

CUR_DIR := $(shell pwd)
BIN_DIR := $(CUR_DIR)/bin
BUILD_DIR := $(CUR_DIR)/build_dir

LIBC := $(CUR_DIR)/newlibc
LIBC_HEADER := $(LIBC)/include
LIBC_OBJECT := $(LIBC)/libc.a
NEMU_HEADER := $(LIBC)/nemu

TESTCASE_OUT_ASM := mov

export CC LD
export LIBC_HEADER LIBC_OBJECT NEMU_HEADER


MAKE := make
INSTALL := install
CP := cp -afu
RM := rm -rf

build/prepare:
	$(INSTALL) -d $(BIN_DIR) $(BUILD_DIR)

build/nemu: build/prepare loader
	$(CP) nemu $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/nemu all
	$(INSTALL) $(BUILD_DIR)/nemu/nemu $(BIN_DIR)

build/loader: build/prepare
	$(CP) loader $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/loader all
	$(INSTALL) $(BUILD_DIR)/loader/loader $(BIN_DIR)

build/testcase/asm: build/prepare
	$(CP) testcase/asm $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/asm all
	cd $(BUILD_DIR)/asm && $(INSTALL) $(TESTCASE_OUT_ASM) $(BIN_DIR)

build/testcase/c: build/prepare
	$(CP) napp/c $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/napp/c all

loader: build/loader
	objcopy -S -O binary $(BIN_DIR)/loader $(BIN_DIR)/loader
	cd $(BIN_DIR) && xxd -i loader > ../nemu/loader/loader.c
	rm $(BIN_DIR)/loader

clean/nemu:
	$(RM) $(BUILD_DIR)/nemu
	$(RM) $(BIN_DIR)/nemu

clean/loader:
	$(RM) $(BUILD_DIR)/loader
	$(RM) $(BIN_DIR)/loader

clean:
	$(RM) $(BIN_DIR) $(BUILD_DIR)

.PHONY: build/prepare build/nemu build/loader loader clean/nemu clean/loader clean

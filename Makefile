

CC := gcc
LD := ld
MAKE := make
INSTALL := install
CP := cp -af
RM := rm -rf


BIN_DIR := bin
BUILD_DIR := build_dir


build/prepare:
	$(INSTALL) -d $(BIN_DIR) $(BUILD_DIR)

nemu/build: build/prepare
	$(CP) nemu $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/nemu all
	$(INSTALL) $(BUILD_DIR)/nemu/nemu $(BIN_DIR)

nemu/loader: nkernel/build
	objcopy -S -O binary $(BIN_DIR)/loader loader
	xxd -i loader > src/elf/loader.c
	rm loader

nemu/clean:
	$(RM) $(BUILD_DIR)/nemu
	$(RM) $(BIN_DIR)/nemu

nkernel/build: build/prepare
	$(CP) nkernel $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/nkernel all
	$(INSTALL) $(BUILD_DIR)/nkernel/loader $(BIN_DIR)

nkernel/clean:
	$(RM) $(BUILD_DIR)/nkernel
	$(RM) $(BIN_DIR)/loader

napp/asm/build: build/prepare
	$(CP) napp/asm $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/napp/asm all

napp/c/build: build/prepare
	$(CP) napp/c $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)/napp/c all

clean:
	$(RM) $(BIN_DIR) $(BUILD_DIR)


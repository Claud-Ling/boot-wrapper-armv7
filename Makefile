# Makefile - build a kernel+filesystem image for stand-alone Linux booting
#
# Copyright (C) 2011 ARM Limited. All rights reserved.
#
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE.txt file.


# Include config file (prefer config.mk, fall back to config-default.mk)
ifneq ($(wildcard config.mk),)
include config.mk
else
include config-default.mk
endif

IMAGE		= linux-stand-alone
OUTPUT		= $(IMAGE).bin
KERNEL		= prebuild/Image
FILESYSTEM	= prebuild/filesystem.cpio.gz

DRIVERS		= drivers/puts.o
OBJS 		= start.o c_start.o string.o kernel_loader.o $(DRIVERS)

LD_SCRIPT	= ./script/linux-stand-alone.lds.S
LD_BOOT_SCRIPT	= ./script/boot.lds.S


# Build all wrappers
all: $(OUTPUT)

clean distclean:
	rm -f bootwrapper* linux-stand-alone.lds boot.lds \
	$(OBJS) $(IMAGE) $(OUTPUT) kernel.s filesystem.s

$(OUTPUT):$(IMAGE)
	$(OBJCOPY) -O binary $< $@

$(IMAGE): linux-stand-alone.lds bootwrapper.s kernel.s filesystem.s Makefile
	$(LD) -M -T linux-stand-alone.lds -o $@ bootwrapper.s kernel.s filesystem.s 

linux-stand-alone.lds: $(LD_SCRIPT)
	$(CC) $(CPPFLAGS) $(IMG_LAYOUT) -E -P -C -o $@ $<

########################bootwrapper################################
#
#
bootwrapper.s:bootwrapper.bin
	$(LD) -T ./script/ld.common.script -b binary -o $@ $^

bootwrapper.bin:bootwrapper
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -xd $< > bootwrapper.dump
bootwrapper:$(OBJS) boot.lds Makefile
	$(LD) -Map bootwrapper.map -o $@ $(OBJS) --script=boot.lds

boot.lds: $(LD_BOOT_SCRIPT) Makefile
	$(CC) $(CPPFLAGS) -DTEXT_BASE=$(TEXT_BASE) -E -P -C -o $@ $<

#######################kernel######################################
#
#
kernel.s:$(KERNEL)
	$(LD) -T ./script/ld.common.script -b binary -o $@ $^

#######################initramfs######################################
#
#
filesystem.s:$(FILESYSTEM)
	$(LD) -T ./script/ld.common.script -b binary -o $@ $^


#######################rules##########################################
#
#
%.o: %.S
	$(CC) $(CPPFLAGS) -DKCMD='$(KCMD)' -c -o $@ $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(IMG_LAYOUT) -O2 -ffreestanding -I. -Iinclude -Ilibfdt -c -o $@ $<

.PHONY: all clean distclean config.mk config-default.mk

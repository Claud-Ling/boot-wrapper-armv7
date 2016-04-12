# Configuration file included in Makefile
#
# Copyright (C) 2011 Columbia University. All rights reserved.
# 		     Christoffer Dall <cdall@cs.columbia.edu>
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE.txt file.
#
# This is a sample configuration file. To make changes, copy this file to
# config.mk and modify that file.
#
# For all systems you can override USE_INITRD and KCMD from the command-line.
#

###########################################################################
# Main options
#
CROSS_COMPILE	?= arm-linux-
ARCH		?= arm

CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

# Select system:
# sx7:
# sx8:
SYSTEM ?= sx7

###########################################################################
# Turn this on to use an initrd whose contents are in filesystem.cpio.gz
USE_INITRD ?= yes
ifeq ($(USE_INITRD),yes)
CPPFLAGS	+= -DUSE_INITRD
INITRAMFS	?= ./filesystem.cpio.gz
else
INITRAMFS	 =
endif

###########################################################################
# Default NFS root
NFS_ROOT	?= /srv/nfsroot
ifeq ($(origin NFS_SERVER), undefined)
NFS_SERVER	:= $(shell ip addr show scope global | \
		   sed -ne '/inet/{s/ *inet \([^/]*\)\/.*/\1/p;q}')
endif


###########################################################################
# Prosx7
#
ifeq ($(SYSTEM),sx7)
USE_INITRD 	= yes

KERNEL_ENTRY	:= 0x8000
KERNEL_SIZE	:= 0xa00000
INITRAMFS_ENTRY := 0xd00000
INITRAMFS_SIZE	:= 0x1000000
TEXT_BASE	:= 0x1000

CPPFLAGS	+= -DMACH_SX7
CPPFLAGS	+= -march=armv7-a -marm
#CPPFLAGS	+= -DTHUMB2_KERNEL
IMG_LAYOUT	+= -DKERNEL_ENTRY=$(KERNEL_ENTRY) \
		   -DKERNEL_SIZE=$(KERNEL_SIZE) \
		   -DINITRAMFS_ENTRY=$(INITRAMFS_ENTRY) \
		   -DINITRAMFS_SIZE=$(INITRAMFS_SIZE)   \
		   -DTEXT_BASE=$(TEXT_BASE)

# Default kernel command line, using initrd:
ifeq ($(USE_INITRD),yes)
	KCMD ?= "/dev/ram0 rw mem=188m console=ttyS0,115200n8 console=ttyS0,115200n8 earlyprintk=serial,uart0,115200"
endif
#
# Default kernel command line, without initrd:
ifneq ($(USE_INITRD),yes)
	KCMD ?= "console=ttyS0 mem=512M mem=512M@0x880000000 earlyprintk"
endif
endif # SYSTEM = SX7

###########################################################################
# ProSX8
#
ifeq ($(SYSTEM),sx8)
USE_INITRD 	= yes

KERNEL_ENTRY	:= 0x8000
KERNEL_SIZE	:= 0xa00000
INITRAMFS_ENTRY := 0xd00000
INITRAMFS_SIZE	:= 0x1000000
TEXT_BASE	:= 0x1000

CPPFLAGS	+= -DMACH_SX8
CPPFLAGS	+= -march=armv7-a -marm
#CPPFLAGS       += -DTHUMB2_KERNEL
IMG_LAYOUT	+= -DKERNEL_ENTRY=$(KERNEL_ENTRY) \
		   -DKERNEL_SIZE=$(KERNEL_SIZE) \
		   -DINITRAMFS_ENTRY=$(INITRAMFS_ENTRY) \
		   -DINITRAMFS_SIZE=$(INITRAMFS_SIZE)	\
		   -DTEXT_BASE=$(TEXT_BASE)

# Default kernel command line, using initrd:
ifeq ($(USE_INITRD),yes)
	KCMD ?= "/dev/ram0 rw mem=188m console=ttyS0,115200n8 console=ttyS0,115200n8 earlyprintk=serial,uart0,115200"
endif
#
# Default kernel command line, without initrd:
ifneq ($(USE_INITRD),yes)
	KCMD ?= "console=ttyS0 mem=512M mem=512M@0x880000000 earlyprintk"
endif
endif # SYSTEM = SX8

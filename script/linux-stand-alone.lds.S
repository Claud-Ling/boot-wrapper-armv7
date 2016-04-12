/*
 * linux-stand-alone.lds.S - simple linker script for stand-alone Linux booting
 *
 * Copyright (C) 2011 ARM Limited. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
TARGET(binary)



SECTIONS
{
	. = 0;
	.text :
	{
		. = TEXT_BASE;
		. = ALIGN(4);
		bootwrapper.s(*) 
		. = KERNEL_ENTRY;
		kernel.s(*)
		. = KERNEL_ENTRY + KERNEL_SIZE;
		. = INITRAMFS_ENTRY;
		filesystem.s(*)
		. = INITRAMFS_ENTRY + INITRAMFS_SIZE;
	}

}

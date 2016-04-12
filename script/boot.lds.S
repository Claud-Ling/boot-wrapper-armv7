/*
 * boot.lds.S - simple linker script for stand-alone Linux booting
 *
 * Copyright (C) 2011 ARM Limited. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
TARGET(binary)


PHYS_OFFSET = 0x00000000;
STACKTOP = 0x7000;


SECTIONS
{
 . = PHYS_OFFSET;
 . = PHYS_OFFSET + 0x1000;
 .text : { *(.text) }
 .data : { *(.data) }
 .bss : { *(.bss) }

 . = STACKTOP;
 stacktop = .;
}

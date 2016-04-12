/*
 * boot.S - simple register setup code for stand-alone Linux booting
 *
 * Copyright (C) 2011 ARM Limited. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

	.syntax	unified
	.arch_extension sec
	.arch_extension virt
	.text

.align 5
/* We use the same vector table for Hyp and Monitor mode, since
 * we will only use each once and they don't overlap.
 */
.globl _start
_start:
	b reset	/* reset */
	.word 0	/* undef */
	.word 0 /* smc */
	.word 0 /* pabt */
	.word 0 /* dabt */
	.word 0 /* not use */
	.word 0 /* irq */
	.word 0 /* fiq */

reset:
	@b	reset
	/*         Disable I and F, set SVC mode     */
	mrs r0,cpsr
	bic r0,r0,#0x1f
	orr r0,r0,#0xd3
	msr cpsr,r0

	/*           cpu initialization              */
	/* Disable caches, MMU and branch prediction */ 
	mrc p15, 0, r0, c1, c0, 0
	/* clear bits 7, 2:0 (B--- -CAM) */
	bic r0, r0, #0x00000087  
	/* clear bits 11,12 (ZI) */
	bic r0, r0, #0x00001800
	/* set bit 13 (V) */
	orr r0, r0, #0x00002000   
	mcr p15, 0, r0, c1, c0, 0

	@ Now we've got rid of the secondary CPUs, set up a stack
	@ for CPU 0 so we can write most of this in C.
	ldr     sp, =stacktop

	@ And call the C entrypoint
	ldr	r0, _c_start
	bx	r0
	@ Never reached
_c_start:
	.word	c_start



.globl __boot_kernel
__boot_kernel:
	mov	r4, r0
	stmfd	sp!, {r1-r3}
	ldmia	sp, {r0-r3}

	bx	r4
.type __boot_kernel, %function

	@
	@ Data
	@
	/* The kernel boot command line for builtin kernels is defined in the Make system */
	.globl kernel_cmd
	.globl kernel_cmd_end
kernel_cmd:
#ifdef KCMD
	.asciz KCMD
#endif
kernel_cmd_end:

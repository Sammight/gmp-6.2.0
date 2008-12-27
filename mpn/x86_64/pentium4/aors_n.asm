dnl  x86-64 mpn_add_n/mpn_sub_n optimized for Pentium 4.

dnl  Copyright 2007 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.

dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 3 of the License, or (at
dnl  your option) any later version.

dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

include(`../config.m4')


C	     cycles/limb
C K8,K9:	 2.8
C K10:		 2.8
C P4:		 4
C P6-15:	 3.6-5	(fluctuating)


C INPUT PARAMETERS
define(`rp',	`%rdi')
define(`up',	`%rsi')
define(`vp',	`%rdx')
define(`n',	`%rcx')
define(`cy',	`%r8')

ifdef(`OPERATION_add_n', `
	define(ADDSUB,	      add)
	define(func,	      mpn_add_n)
	define(func_nc,	      mpn_add_nc)')
ifdef(`OPERATION_sub_n', `
	define(ADDSUB,	      sub)
	define(func,	      mpn_sub_n)
	define(func_nc,	      mpn_sub_nc)')

MULFUNC_PROLOGUE(mpn_add_n mpn_add_nc mpn_sub_n mpn_sub_nc)

ASM_START()

	TEXT
	ALIGN(16)

PROLOGUE(func_nc)
	jmp	L(ent)
EPILOGUE()

PROLOGUE(func)
	xor	%r8, %r8
L(ent):	push	%rbx
	push	%r12

	mov	%ecx, %eax
	and	$3, %eax
	jne	L(n00)		C n = 0, 4, 8, ...
	mov	%r8, %rbx
	mov	(up), %r8
	mov	(vp), %r9
	mov	8(up), %r10
	ADDSUB	%r9, %r8
	mov	8(vp), %r9
	setc	%al
	lea	-16(rp), rp
	sub	$4, n
	jmp	L(L00)

L(n00):	cmp	$2, %eax
	jnc	L(n01)		C n = 1, 5, 9, ...
	mov	(up), %r11
	mov	(vp), %r9
	mov	%r8, %rax
	xor	%ebx, %ebx
	sub	$4, n
	jnc	L(gt1)
	ADDSUB	%r9, %r11
	setc	%bl
	ADDSUB	%rax, %r11
	adc	$0, %ebx
	mov	%r11, (rp)
	jmp	L(ret)
L(gt1):	mov	8(up), %r8
	ADDSUB	%r9, %r11
	mov	8(vp), %r9
	setc	%bl
	lea	-8(rp), rp
	lea	8(up), up
	lea	8(vp), vp
	jmp	L(L01)

L(n01):	jne	L(n10)		C n = 2, 6, 10, ...
	mov	(up), %r12
	mov	(vp), %r9
	mov	%r8, %rbx
	mov	8(up), %r11
	ADDSUB	%r9, %r12
	mov	8(vp), %r9
	setc	%al
	ADDSUB	%rbx, %r12
	jc	L(c3a)
L(rc3a):	lea	16(up), up
	lea	16(vp), vp
	sub	$4, n
	jnc	L(top)
	jmp	L(end)

L(n10):	mov	(up), %r10	C n = 3, 7, 11, ...
	mov	(vp), %r9
	mov	%r8, %rax
	xor	%ebx, %ebx
	mov	8(up), %r12
	ADDSUB	%r9, %r10
	mov	8(vp), %r9
	setc	%bl
	lea	-24(rp), rp
	lea	-8(up), up
	lea	-8(vp), vp
	jmp	L(L11)

L(c3a):	mov	$1, %al
	jmp	L(rc3a)
L(c0):	mov	$1, %bl
	jmp	L(rc0)
L(c1):	mov	$1, %al
	jmp	L(rc1)
L(c2):	mov	$1, %bl
	jmp	L(rc2)
L(c3):	mov	$1, %al
	jmp	L(rc3)

	ALIGN(16)
L(top):	mov	(up), %r8	C not on critical path
	ADDSUB	%r9, %r11	C not on critical path
	mov	(vp), %r9	C not on critical path
	setc	%bl		C save carry out
	mov	%r12, (rp)
L(L01):	ADDSUB	%rax, %r11	C apply previous carry out
	jc	L(c0)		C jump if ripple
L(rc0):	mov	8(up), %r10
	ADDSUB	%r9, %r8
	mov	8(vp), %r9
	setc	%al
	mov	%r11, 8(rp)
L(L00):	ADDSUB	%rbx, %r8
	jc	L(c1)
L(rc1):	mov	16(up), %r12
	ADDSUB	%r9, %r10
	mov	16(vp), %r9
	setc	%bl
	mov	%r8, 16(rp)
L(L11):	ADDSUB	%rax, %r10
	jc	L(c2)
L(rc2):	mov	24(up), %r11
	ADDSUB	%r9, %r12
	lea	32(up), up
	mov	24(vp), %r9
	lea	32(vp), vp
	setc	%al
	mov	%r10, 24(rp)
L(L10):	ADDSUB	%rbx, %r12
	jc	L(c3)
L(rc3):	lea	32(rp), rp
	sub	$4, n
	jnc	L(top)

L(end):	ADDSUB	%r9, %r11
	setc	%bl
	mov	%r12, (rp)
	ADDSUB	%rax, %r11
	jnc	L(1)
	mov	$1, %bl
L(1):	mov	%r11, 8(rp)

L(ret):	mov	%ebx, %eax
	pop	%r12
	pop	%rbx
	ret
EPILOGUE()

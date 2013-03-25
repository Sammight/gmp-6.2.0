dnl  IA-64 mpn_addlsh1_n, mpn_sublsh1_n, mpn_rsblsh1_n

dnl  Contributed to the GNU project by Torbjörn Granlund.

dnl  Copyright 2003, 2004, 2005, 2010, 2013 Free Software Foundation, Inc.

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

C           cycles/limb
C Itanium:      3.0
C Itanium 2:    1.5


define(LSH,		1)

ifdef(`OPERATION_addlsh1_n',`define(`DO_add')')
ifdef(`OPERATION_sublsh1_n',`define(`DO_sub')')
ifdef(`OPERATION_rsblsh1_n',`define(`DO_rsb')')

MULFUNC_PROLOGUE(mpn_addlsh1_n mpn_sublsh1_n mpn_rsblsh1_n)

include_mpn(`ia64/aorsorrlshC_n.asm')
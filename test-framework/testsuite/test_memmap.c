/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (c) HCL Technologies Limited, 2020
 
 * Author:Arjun Kumar Rath <arjun-r@hcl.com>
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

 */
#include "../common/process_diagtest_cmds.h"

int do_boarddiag (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

extern uint32_t __image_copy_start;
extern uint32_t __bss_end;

TestResultTypes_e t_memmap( uint32_t testId, void * arg )
{
	uint32_t stack_variable = 3;
	char * string_pointer = "Hello";
	char a[1024];

	printf( "*** stack ***\n");
	printf( "%20s %08X\n", "(&stack_variable)", (uint32_t)(&stack_variable));
	printf( "%20s %08X\n", "&string_pointer", (uint32_t)&string_pointer);
	printf( "%20s %08X\n", "&a", (uint32_t)&a);

	printf( "*** code ***\n");
	printf( "%20s %08X\n", "&__image_copy_start", (uint32_t)(&__image_copy_start));
	printf( "%20s %08X\n", "do_boarddiag", (uint32_t)do_boarddiag);
	printf( "%20s %08X\n", "t_memmap", (uint32_t)t_memmap);

	printf( "*** const string ***\n");
	printf( "%20s %08X\n", "string_variable", (uint32_t)string_pointer);
	printf( "%20s %08X\n", "(string_pointer+1)", (uint32_t)(string_pointer+1));
	printf( "%20s %08X\n", "(string_pointer+2)", (uint32_t)(string_pointer+2));
	printf( "%20s %08X\n", "&__bss_end", (uint32_t)(&__bss_end));

	printf( "*** junk ***\n");
	printf( "%20s %08X\n", "__image_copy_start", (uint32_t)__image_copy_start);
	printf( "%20s %08X\n", "stack_variable", (uint32_t)stack_variable);
	printf( "%20s %08X\n", "*string_pointer", (uint32_t)(*string_pointer));

	return DIAG_TEST_RESULT_PASS;
}


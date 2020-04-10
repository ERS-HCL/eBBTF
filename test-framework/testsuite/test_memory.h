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
#ifndef TEST_MEMORY_H
#define TEST_MEMORY_H

#include "../common/process_diagtest_cmds.h"

TestResultTypes_e memory_data_line_test(uint32_t testId, void * arg);

TestResultTypes_e memory_address_line_test(uint32_t testId, void * arg);

TestResultTypes_e memory_pattern_test(uint32_t testId, void * arg);


typedef struct {
    unsigned long mem_address;

} A_memory_data_line_test;

typedef struct {
    unsigned long mem_address;
    uint32_t size;

} A_memory_address_line_test;

typedef struct {
    unsigned long mem_address;
    uint32_t size;
    uint32_t value;
} A_memory_pattern_test;




#endif

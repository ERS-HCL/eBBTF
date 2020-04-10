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
#include "process_diagtest_cmds.h"
#include "UbootTypes.h"

#include <common.h>

#define FNV_PRIME_32 16777619u
#define FNV_OFFSET_32 2166136261u

unsigned long FNV32(unsigned char *ptr, unsigned long len )
{
    uint32_t hash = FNV_OFFSET_32;
    uint32_t i;
    for(i = 0; i < len; ++i)
    {
        hash = hash ^ (ptr[i]); // xor next byte into the bottom of the hash
        hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
    }
    return hash;
}

unsigned long CalculateTestIdHash( char * name)
{
    uint32_t len = strlen( name );
    uint32_t testId = FNV32( (unsigned char *)name, len );

    return testId;
}

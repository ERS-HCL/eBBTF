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
#ifndef TEST_MMC_H_
#define TEST_MMC_H_



/**
 * MMC Initialization test
 *
 * This test checks the Insertion and Detection of the mmc devices
 *
 */
TestResultTypes_e MMC_Detection_Test( uint32_t testId, void * arg);

typedef struct {
    uint32_t base_address;
} A_MMC_Detection_Test;

TestResultTypes_e MMC_WRITE_READ_Test( uint32_t testId, void * arg);

typedef struct {
    uint32_t base_address;
    uint32_t block_start;
    uint32_t block_cnt;
    uint32_t mmc_test_dev;
} A_MMC_WRITE_READ_Test;

#endif /* TEST_MMC_H_ */

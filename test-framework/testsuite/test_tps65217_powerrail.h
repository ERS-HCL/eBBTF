/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (c) HCL Technologies Limited, 2020
 
 * Author:Titus Dhanasingh  <titus.dhanasingh@hcl.com>
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
#ifndef TEST_TPS65217_PWRRAIL_H
#define TEST_TPS65217_PWRRAIL_H

#include <common.h>
#include "../utils/i2c_utils.h"
#include "../common/process_diagtest_cmds.h"


/**
 * PWR Railings_Test
 *
 * The test reads the power railings voltage values
 *
 */

TestResultTypes_e PWR_railings_test( uint32_t testId, void * arg);

#define MAX_PWR_REGS 128
#define MAX_NAME_LEN 64

/* TPS65217C Registers */
#define PMIC_DCDC1_DDR  0x0E
#define PMIC_DCDC2_MPU  0x0F
#define PMIC_DCDC3_CORE 0x10
#define PMIC_LD01       0x12
#define PMIC_LD02       0x13
#define PMIC_LD03       0x14
#define PMIC_LD04       0x15

#define PMIC_STATUS_FLAGS 0x0C

#define PMIC_REGULATION_REGS 7

typedef struct {
    int8_t i2c_bus_no;
    uint8_t i2c_dev_addr;
    const char dev_name[MAX_NAME_LEN]; 
}A_PWR_railings_test;

#endif

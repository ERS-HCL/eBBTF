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
#ifndef TEST_DS2321RTC_H
#define TEST_DS2321RTC_H


TestResultTypes_e ds2321_rtc_get_time_test(uint32_t testId, void *arg);

#define I2C1_SCL_OFFSET   0x44e10980
#define I2C1_SDA_OFFSET   0x44e10984

#define I2C1_SCL_MODE3    0x0000007B
#define I2C1_SDA_MODE3    0x0000007B


typedef struct
{
    uint8_t i2c_bus;
    uint32_t i2c_speed;
    uint8_t i2c_dev_addr;
}A_ds2321_rtc_get_time_test;


TestResultTypes_e ds2321_rtc_set_time_test(uint32_t testId, void *arg);

typedef struct
{
    uint8_t i2c_bus;
    uint32_t i2c_speed;
    uint8_t i2c_dev_addr;
    uint8_t sec_value;
    uint8_t min_value;
    uint8_t hour_value;
}A_ds2321_rtc_set_time_test;


#endif /* TEST_DS2321RTC_H */

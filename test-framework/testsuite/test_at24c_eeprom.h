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
#ifndef TEST_AT24C_EEPROM_H
#define TEST_AT24C_EEPROM_H

TestResultTypes_e   EEPROM_Read_Write_test(uint32_t testId, void *arg);

#define EEPROM_TEST_OFFSET  128 
#define EEPROM_TEST_LEN     128 
#define TEST_BUFFER_LEN     128 
#define EEPROM_OFFSET_LEN   2
#define EEPROM_TEST_PATTERN 0xEA
#define EEPROM_PAGE_LEN     32

#define I2C1_SCL_OFFSET   0x44e10980
#define I2C1_SDA_OFFSET   0x44e10984

#define I2C1_SCL_MODE3    0x0000007B
#define I2C1_SDA_MODE3    0x0000007B

#define EEPROM_DEV_ADDR   0x57

typedef struct {
    uint8_t  i2c_bus_num;
    uint8_t  i2c_dev_addr;
    uint8_t  eeprom_offset;
    uint8_t  test_len;
}A_EEPROM_Read_Write_test;


#endif /* TEST_EEPROM_H */

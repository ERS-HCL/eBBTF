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
#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include "../common/process_diagtest_cmds.h"
#include <i2c.h>

#define I2C_BUS_0                   0
#define I2C_BUS_1                   1
#define I2C_BUS_2                   2
#define I2C_BUS_3                   3

#define I2C_SPEED_100Khz            (100 * 1000)
#define I2C_SPEED_400Khz            (400 * 1000)

TestResultTypes_e eBBTF_utils_i2c_bus_init( uint32_t i2c_speed, uint8_t i2c_bus);
TestResultTypes_e eBBTF_utils_i2c_write(uint8_t bus, unsigned int chip_address, uint8_t reg,int addr_offset_len,
                                            uint8_t *write_value, uint8_t dlen);
TestResultTypes_e eBBTF_utils_i2c_read(uint8_t bus, unsigned int chip_address, uint8_t reg,int addr_offset_len,
                                            uint8_t *read_value, uint8_t dlen);
int eBBTF_utils_set_i2c_bus(unsigned int busnum);
int eBBTF_utils_get_i2c_bus(struct udevice **busp);
int eBBTF_utils_i2c_bus_speed(int bus_speed);
#endif

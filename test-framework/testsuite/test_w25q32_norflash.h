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
#ifndef TEST_W25Q32_NORFLASH_H
#define TEST_W25Q32_NORFLASH_H

#include <common.h>
#include "../common/process_diagtest_cmds.h"


/**
 * SPI NOR flash 
 *
 * The test will verify erase, read and write operations
 *
 */

TestResultTypes_e NOR_Flash_test( uint32_t testId, void * arg);

#define SPI_TEST_LEN       (128 * 1024) /* 128 KB */

#define PIN_MUX_CFG     0
#define PIN_MUX_RESTORE 1 

#define SPI0_CLK_MODE0   0x00000030
#define SPI0_D0_MODE0    0x00000030
#define SPI0_D1_MODE0    0x00000010
#define SPI0_CS0_MODE0   0x00000010

#define SPI0_CLK_OFFSET  0x44e10950
#define SPI0_D0_OFFSET   0x44e10954
#define SPI0_D1_OFFSET   0x44e10958
#define SPI0_CS0_OFFSET  0x44e1095C 

typedef struct {
    uint8_t spi_bus_no;
    uint8_t spi_cs_no;
    uint32_t spi_offset;
    uint32_t spi_len; 
}A_NOR_Flash_test;

#endif

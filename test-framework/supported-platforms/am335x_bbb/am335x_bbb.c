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
#include "../../common/process_diagtest_cmds.h"


#include "../../testsuite/test_i2cprobe.h"

INVOKE_TEST_WITH_PARAM( am335x_bbb, "I2C bus 0 probe", I2C_interface_test, I2C0_DEVICES) = {
        I2C_BUS_0,                               // I2C Bus Number
        {0x24,0x34,0x50},                       // I2C Device Address list
        {"Slave 1", "Slave 2", "Slave 3"},                      // I2C Device Name list
        3                                        // Peripheral Count
};

INVOKE_TEST_WITH_PARAM( am335x_bbb, "I2C bus 1 probe", I2C_interface_test, I2C1_DEVICES) = {
        I2C_BUS_1,                               // I2C Bus Number
        {0x24,0x34,0x50},                       // I2C Device Address list
        {"Slave 1", "Slave 2", "Slave 3"},                      // I2C Device Name list
        3                                        // Peripheral Count
};


#include "../../testsuite/test_ping.h"
INVOKE_TEST_WITH_PARAM( am335x_bbb, "Ping", ping_test, smsc95xx_eth) = {
   "10.144.172.135"
};



#include "../../testsuite/test_mmc.h"
#define BBB_MMC_BASE_ADDR       0x481D8000

INVOKE_TEST_WITH_PARAM(am335x_bbb, "eMMC presence", MMC_Detection_Test, eMMC_check) = {
        BBB_MMC_BASE_ADDR   /* eMMC device */
};

#define EMMC_TEST_BLOCK_START     583 
#define EMMC_TEST_BLOCK_CNT       16 
#define EMMC_TEST_DEVICE          0

INVOKE_TEST_WITH_PARAM(am335x_bbb, "eMMC Read, Write and Erase tests", 
                MMC_WRITE_READ_Test, eMMC_Access_Check) = {
        BBB_MMC_BASE_ADDR,   /* eMMC device */
        EMMC_TEST_BLOCK_START,
        EMMC_TEST_BLOCK_CNT,
        EMMC_TEST_DEVICE
};


#include "../../testsuite/test_memory.h"
INVOKE_TEST_WITH_PARAM(am335x_bbb, "Memory Test", memory_data_line_test,Data_Line) = {
        0x88080000,//address
};

INVOKE_TEST_WITH_PARAM(am335x_bbb, "Memory Test", memory_address_line_test,Address_Line) = {
        0x88080000,//address
        2048       // size of area to test in bytes
};

INVOKE_TEST_WITH_PARAM(am335x_bbb, "Memory Test", memory_pattern_test,Pattern) = {
        0x88080000,// address
        2048,       // size of area to test in bytes
        0x55555555
};


#define PAD_CTRL_BASE_ADDR 		0x44E10000
#define PAD_CTRL_OFFSET_1 		0x814
#define GPIO_BANK_BASE_ADDR		0x4804c000
#define GPIO_BANK_ENABLE_OFFSET		0x130
#define GPIO_BANK_OUTPUT_OFFSET		0x134
#define GPIO_BANK_DATAIN_OFFSET         0x138
#define GPIO_BANK_DATAOUT_OFFSET        0x13C

		
#include "../../testsuite/test_gpio.h"
INVOKE_TEST_WITH_PARAM(am335x_bbb, "GPIO Output Enable", GPIO_test, GPIO0_OUTPUT_ENABLE) = {
	PAD_CTRL_BASE_ADDR,
	PAD_CTRL_OFFSET_1,
	0x17,
	GPIO_BANK_BASE_ADDR,
	GPIO_BANK_ENABLE_OFFSET,
	0x0,
	1,
	GPIO_BANK_OUTPUT_OFFSET,
	GPIO_BANK_DATAIN_OFFSET,
	GPIO_BANK_DATAOUT_OFFSET,
	21,
	1        
};

INVOKE_TEST_WITH_PARAM(am335x_bbb, "GPIO Output Disable", GPIO_test, GPIO0_OUTPUT_DISABLE) = {
        PAD_CTRL_BASE_ADDR,
        PAD_CTRL_OFFSET_1,
        0x17,
        GPIO_BANK_BASE_ADDR,
        GPIO_BANK_ENABLE_OFFSET,
        0x0,
        1,
        GPIO_BANK_OUTPUT_OFFSET,
        GPIO_BANK_DATAIN_OFFSET,
        GPIO_BANK_DATAOUT_OFFSET,
        21,
        0
};


#include "../../testsuite/test_eth.h"
INVOKE_TEST_WITH_PARAM( am335x_bbb, "Ethernet Test", Ethernet_test, SPEED_DUPLEX_AUTONEGOTIATE) = {
        {"ethernet@4a100000"},                 // Ethernet Device 
        0x00,                                  // Phy Address
        {"LAN8710A"}                           // Ethernet Phy 
};

#include "../../testsuite/test_tps65217_powerrail.h"
INVOKE_TEST_WITH_PARAM( am335x_bbb, "PWR railings display", PWR_railings_test, REGISTER_READ) = {
        I2C_BUS_0,                               // I2C Bus Number
        0x24,                                  // I2C Device Address
        {"TPS65217C"},                        // Power Management IC Name
};

#include "../../testsuite/test_at24c_eeprom.h"
INVOKE_TEST_WITH_PARAM( am335x_bbb, "EEPROM Read Write", EEPROM_Read_Write_test, EEPROM) = {
        I2C_BUS_1,                               // I2C Bus Number
        0x50,                                    // I2C Device Address list
        EEPROM_TEST_OFFSET,                      // OFFSET -Read Write
        EEPROM_TEST_LEN                          // Area used by test 
};

#include "../../testsuite/test_w25q32_norflash.h"
INVOKE_TEST_WITH_PARAM( am335x_bbb, "SPI NOR flash erase-Write-Read test ", NOR_Flash_test,NOR_FLASH) = {
        0x00,                                  // SPI Bus No 
        0x00,                                  // SPI CS No
        0x00,                                  // SPI Offset
        0x20000                                // Test Len 
};

#include "../../testsuite/test_ds2321_rtc.h"
#define DS2321_RTC_SLAVE_ADDRESS    0x68
#define DS2321_RTC_SEC_VALUE        0x20
#define DS2321_RTC_MIN_VALUE        0x14
#define DS2321_RTC_HR_VALUE         0x05

INVOKE_TEST_WITH_PARAM( am335x_bbb, "RTC set time", ds2321_rtc_set_time_test, value) = {
        I2C_BUS_1,                  /* I2C bus number               */
        I2C_SPEED_100Khz,           /* I2C bus speed : 100kHz       */
        DS2321_RTC_SLAVE_ADDRESS,   /* ds2321_rtc_slave address    */
        DS2321_RTC_SEC_VALUE,       /*  test the sec_value */
        DS2321_RTC_MIN_VALUE,       /*  test the min_value */
        DS2321_RTC_HR_VALUE,        /*  test the hr_value */
};

INVOKE_TEST_WITH_PARAM( am335x_bbb, "RTC get time", ds2321_rtc_get_time_test, value) = {
        I2C_BUS_1,                  /* I2C bus number               */
        I2C_SPEED_100Khz,           /* I2C bus speed : 100kHz       */
        DS2321_RTC_SLAVE_ADDRESS,   /* ds2321_rtc_slave address    */
};





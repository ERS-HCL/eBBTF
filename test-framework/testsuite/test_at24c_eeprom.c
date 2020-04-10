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
#include "../common/process_diagtest_cmds.h"
#include "test_at24c_eeprom.h"
#include "../utils/i2c_utils.h"
#include <asm/io.h>
#include <i2c.h>
#include <common.h>
#include <asm/omap_common.h>
#include <dm/uclass.h>
#include <i2c.h>

unsigned char i2c_eeprom_testbuf1[TEST_BUFFER_LEN];
unsigned char i2c_eeprom_testbuf2[TEST_BUFFER_LEN];

//TODO : This needs to move to different folder as
// the uboot does not support directly the I2C1 bus
// for Beagle bone evaluation platform header
// Enable support in u-boot device tree also
void cfg_exp_i2c1_pin_mux(void)
{
     /* Configure for I2C1 Bus */
     writel(I2C1_SCL_MODE3, I2C1_SCL_OFFSET);
     writel(I2C1_SDA_MODE3, I2C1_SDA_OFFSET);
}

TestResultTypes_e EEPROM_Read_Write_test (uint32_t testId, void * arg)
{
    TestResultTypes_e status       = DIAG_TEST_RESULT_PASS;
    A_EEPROM_Read_Write_test *a = (A_EEPROM_Read_Write_test *) arg;
    int offset;

    // Check for arguments being passed
    if (NULL == a) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

    //TODO - To support Beagle bone Evaluation platform
    /* Prepare pin MUX for expansion baord I2C BUS 1 */
    cfg_exp_i2c1_pin_mux();
    
    /* Prepare test buffer */
    memset(i2c_eeprom_testbuf1, EEPROM_TEST_PATTERN, TEST_BUFFER_LEN);

    for(offset = 0; offset < TEST_BUFFER_LEN; offset += EEPROM_PAGE_LEN) 
    {

	status = eBBTF_utils_i2c_write (a->i2c_bus_num,a->i2c_dev_addr,a->eeprom_offset + offset,EEPROM_OFFSET_LEN,
			&i2c_eeprom_testbuf1[offset]  , EEPROM_PAGE_LEN);

        if(status != DIAG_TEST_RESULT_PASS)
		break;
    }

    if(status != DIAG_TEST_RESULT_PASS) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_WRITE_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus_num;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = a->eeprom_offset;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }


    for(offset = 0; offset < TEST_BUFFER_LEN; offset += EEPROM_PAGE_LEN) {
	 status = eBBTF_utils_i2c_read (a->i2c_bus_num,a->i2c_dev_addr,a->eeprom_offset + offset, EEPROM_OFFSET_LEN,
				&i2c_eeprom_testbuf2[offset], EEPROM_PAGE_LEN);
	 if(status != DIAG_TEST_RESULT_PASS)
		 break;
    }

    if(status != DIAG_TEST_RESULT_PASS) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_READ_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus_num;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = a->eeprom_offset;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    if(memcmp(i2c_eeprom_testbuf2, i2c_eeprom_testbuf1, TEST_BUFFER_LEN)) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_BUS_DATA_MISMATCH_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus_num;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = a->eeprom_offset;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }
    
	return DIAG_TEST_RESULT_PASS;
}

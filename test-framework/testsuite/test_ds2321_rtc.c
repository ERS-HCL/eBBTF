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
#include <common.h>
#include <div64.h>
#include <dm.h>
#include <malloc.h>
#include <mapmem.h>
#include <asm/io.h>
#include <common.h>
#include <asm/omap_common.h>
#include <dm/device-internal.h>

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>
#include <stdbool.h>
#include <linux/delay.h>

#include "../common/process_diagtest_cmds.h"
#include "test_ds2321_rtc.h"
#include "../utils/i2c_utils.h"

#include <bcd.h>
#include <i2c.h>

/*
 * DS2321 RTC register addresses
 */
#define DS2321_RTC_SEC_REG_ADDR         0x00
#define DS2321_RTC_MIN_REG_ADDR         0x01
#define DS2321_RTC_HR_REG_ADDR          0x02

#define DS2321_RTC_CTL_REG_ADDR         0x0E
#define DS2321_RTC_CTL_STATUS_REG_ADDR  0x0F

#define DS2321_SELECT_EXT_CLK            0x04
#define DS2321_SELECT_EXT_32KHZ          0x18

//TODO : This needs to move to different folder as
// the uboot does not support directly the I2C1 bus
// for Beagle bone evaluation platform header
// Enable support in u-boot device tree also

static void cfg_exp_i2c1_pin_mux(void)
{
     /* Configure for I2C1 Bus */
     writel(I2C1_SCL_MODE3, I2C1_SCL_OFFSET);
     writel(I2C1_SDA_MODE3, I2C1_SDA_OFFSET);
}

TestResultTypes_e ds2321_rtc_init(uint32_t testId, uint8_t chip_address, uint8_t bus,
                                                                uint32_t speed)
{
    uint8_t val = 0;
    int8_t setbus_status = 0;

    /* Set Pin Mux for RTC I2C1 over expansion board - P9 */
	//TODO - To support Beagle bone Evaluation platform
    /* Prepare pin MUX for expansion baord I2C BUS 1 */
    cfg_exp_i2c1_pin_mux();

    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
	setbus_status = eBBTF_utils_set_i2c_bus(bus);

    if (setbus_status != 0) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_BUS_NOT_FOUND_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = bus;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

	setbus_status = eBBTF_utils_i2c_bus_speed(speed);
    if (setbus_status < 0) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_BUS_INIT_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = bus;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    /* control/staus reg(Addr 0x0F) */
    status = eBBTF_utils_i2c_read (bus, chip_address,DS2321_RTC_CTL_STATUS_REG_ADDR,1,
                                    &val, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_READ_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = bus;
        event.address[1] = chip_address;
        event.address[2] = DS2321_RTC_CTL_STATUS_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    /* DS2321 CLKIN/INTA pin as input to select external clock source; [Set bit2] */
    val |= DS2321_SELECT_EXT_CLK;

    /*set ext clock freq to 32Khz */
    val |= DS2321_SELECT_EXT_32KHZ;

    status = eBBTF_utils_i2c_write (bus, chip_address, DS2321_RTC_CTL_STATUS_REG_ADDR,1,
                                    &val, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_WRITE_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = bus;
        event.address[1] = chip_address;
        event.address[2] = DS2321_RTC_CTL_STATUS_REG_ADDR;
        return DIAG_TEST_RESULT_FAIL;
    }
    return status;
}

/* read time*/
TestResultTypes_e ds2321_rtc_get_time_test(uint32_t testId, void * arg)
{
    TestResultTypes_e status   = DIAG_TEST_RESULT_PASS;
    uint8_t get_value   = 0;
    uint8_t sec         = 0;
    uint8_t min         = 0;
    uint8_t hour        = 0;
    char buff[256];
    A_ds2321_rtc_get_time_test *a = (A_ds2321_rtc_get_time_test *) arg;

    if(NULL == a) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }
    status = ds2321_rtc_init(testId, a->i2c_dev_addr, a->i2c_bus, a->i2c_speed);
    if (DIAG_TEST_RESULT_PASS != status)
        return DIAG_TEST_RESULT_FAIL;

    status = eBBTF_utils_i2c_read (a->i2c_bus, a->i2c_dev_addr, DS2321_RTC_SEC_REG_ADDR,1,
                                &get_value, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_READ_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = DS2321_RTC_SEC_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }
    
    /* convert from binary-coded decimal (BCD)
     * format to binary format
     */
    get_value &= 0x7f; /* consider only the LSB 7 bits */
    sec = bcd2bin(get_value);

    get_value = 0;
    status = eBBTF_utils_i2c_read (a->i2c_bus, a->i2c_dev_addr, DS2321_RTC_MIN_REG_ADDR,1,
                                &get_value, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_READ_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = DS2321_RTC_MIN_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }
    get_value &= 0x7f; /* consider only the LSB 7 bits */
    min = bcd2bin(get_value );

    get_value = 0;
    status = eBBTF_utils_i2c_read (a->i2c_bus, a->i2c_dev_addr, DS2321_RTC_HR_REG_ADDR,1,
                                &get_value, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_READ_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = DS2321_RTC_HR_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }
    get_value &= 0x7f; /* consider only the LSB 7 bits */
    hour = bcd2bin(get_value);

    sprintf(buff,"RTC_Get_Time [HH:MM:SS] = %d:%d:%d", hour, min,sec);
    printf( "  %s\n", buff);

    return status;
}

/* Set the time */
TestResultTypes_e ds2321_rtc_set_time_test(uint32_t testId, void *arg)
{
    char buff[256];
    TestResultTypes_e status   = DIAG_TEST_RESULT_PASS;
    uint8_t set_value = 0;
    A_ds2321_rtc_set_time_test *a = (A_ds2321_rtc_set_time_test *) arg;
    if(NULL == a) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

    if ((59 < a->sec_value) ||
        (59 < a->min_value) || (23 < a->hour_value)) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

    status = ds2321_rtc_init(testId, a->i2c_dev_addr, a->i2c_bus, a->i2c_speed);
    if (DIAG_TEST_RESULT_PASS != status)
        return DIAG_TEST_RESULT_FAIL;

    /* converts the Bin value into BCD format and write the
     * BCD values into appropriate register
     */
    set_value = bin2bcd(a->sec_value);
    status = eBBTF_utils_i2c_write (a->i2c_bus, a->i2c_dev_addr, DS2321_RTC_SEC_REG_ADDR,1,
                                    &set_value, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_WRITE_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = DS2321_RTC_SEC_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    set_value = bin2bcd(a->min_value);
    status = eBBTF_utils_i2c_write (a->i2c_bus, a->i2c_dev_addr, DS2321_RTC_MIN_REG_ADDR,1,
                                    &set_value, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_WRITE_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = DS2321_RTC_MIN_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    set_value = bin2bcd(a->hour_value);
    status = eBBTF_utils_i2c_write (a->i2c_bus, a->i2c_dev_addr, DS2321_RTC_HR_REG_ADDR,1,
                                    &set_value, 1);
    if (DIAG_TEST_RESULT_PASS != status) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_WRITE_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = a->i2c_bus;
        event.address[1] = a->i2c_dev_addr;
        event.address[2] = DS2321_RTC_HR_REG_ADDR;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    sprintf(buff,"RTC_Get_Time [HH:MM:SS] = %d:%d:%d", a->hour_value, a->min_value, a->sec_value);
    printf( "  %s\n", buff);

    
    return status;
}


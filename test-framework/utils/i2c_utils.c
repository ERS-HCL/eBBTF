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
#include "i2c_utils.h"
#include <dm/uclass.h>
#include <dm/uclass-internal.h>
#include <dm/util.h>
#include <dm/device.h>



#ifdef CONFIG_DM_I2C

struct udevice *i2c_cur_bus;

int eBBTF_utils_i2c_bus_speed(int bus_speed)
{
        int cur_speed, ret=0;

#ifdef CONFIG_DM_I2C
        struct udevice *bus;

        if (eBBTF_utils_get_i2c_bus(&bus))
                return 1;
        cur_speed = dm_i2c_get_bus_speed(bus);
#else
        cur_speed = i2c_get_bus_speed();
#endif
        printf("Current bus speed=%d\n", cur_speed);
        printf("Setting bus speed to %d Hz\n", bus_speed);
#ifdef CONFIG_DM_I2C
        ret = dm_i2c_set_bus_speed(bus, bus_speed);
#else
        ret = i2c_set_bus_speed(bus_speed);
#endif
        if (ret)
        	printf("Failure changing bus speed (%d)\n", ret);
	return ret ;       
}

int eBBTF_utils_set_i2c_bus(unsigned int busnum)
{
        struct udevice *bus;
        int ret;

        ret = uclass_get_device_by_seq(UCLASS_I2C, busnum, &bus);
        if (ret) {
                debug("%s: No bus %d\n", __func__, busnum);
                return ret;
        }
        i2c_cur_bus = bus;

        return 0;
}

int eBBTF_utils_get_i2c_bus(struct udevice **busp)
{
#ifdef CONFIG_I2C_SET_DEFAULT_BUS_NUM
        if (!i2c_cur_bus) {
                if (set_i2c_bus_num(CONFIG_I2C_DEFAULT_BUS_NUMBER)) {
                        printf("Default I2C bus %d not found\n",
                               CONFIG_I2C_DEFAULT_BUS_NUMBER);
                        return -ENODEV;
                }
        }
#endif
        if (!i2c_cur_bus) {
                puts("No I2C bus selected\n");
                return -ENODEV;
        }
        *busp = i2c_cur_bus;
        return 0;
}

#endif

TestResultTypes_e eBBTF_utils_i2c_bus_init( uint32_t i2c_speed, uint8_t i2c_bus)
{
    int8_t i2c_status = 0;
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;

#ifdef CONFIG_DM_I2C
                i2c_status = eBBTF_utils_set_i2c_bus(i2c_bus);
#else
                i2c_status = i2c_set_bus_num(i2c_bus);
#endif

    if (i2c_status < 0) {
        status = DIAG_TEST_RESULT_FAIL;
    }

    return status;
}



TestResultTypes_e eBBTF_utils_i2c_write(uint8_t bus_num, unsigned int chip_address, uint8_t reg,int addr_offset_len,
                                            uint8_t *write_value, uint8_t dlen)
{
	int8_t i2c_status = 0;
	TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
	int ret = 0;

#ifdef CONFIG_DM_I2C
        struct udevice *dev,*bus;
	struct dm_i2c_chip *i2c_chip;
#endif

#ifdef CONFIG_DM_I2C
	i2c_status = eBBTF_utils_set_i2c_bus(bus_num);
#else
	i2c_status = i2c_set_bus_num(bus_num);
#endif
	if (i2c_status != 0) {
        struct DiagEventParams_s event;
        Event_Init( 0, &event);
        event.eventType = DIAG_BUS_NOT_FOUND_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = bus_num;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

#ifdef CONFIG_DM_I2C
        i2c_status = eBBTF_utils_get_i2c_bus(&bus);
        if (i2c_status)
                return DIAG_TEST_RESULT_FAIL;
        ret = i2c_get_chip(bus, chip_address, addr_offset_len, &dev);
        if (!ret)
                ret = i2c_set_chip_offset_len(dev, addr_offset_len);
        i2c_chip = dev_get_parent_platdata(dev);
	i2c_chip->flags |= DM_I2C_CHIP_WR_ADDRESS;
        ret = dm_i2c_write(dev, reg, write_value, dlen);
#else
        ret = i2c_write(chip_address, reg, addr_offset_len, write_value, dlen);
#endif
        if (ret)
	{
		struct DiagEventParams_s event;
        	Event_Init( 0, &event);
        	event.eventType = DIAG_DEV_WRITE_ERR_EVENT;
        	event.addressDim = 1;
        	event.address[0] = bus_num;
        	ReportEvent( &event);
        	return DIAG_TEST_RESULT_FAIL;
	}

    return status;
}


TestResultTypes_e eBBTF_utils_i2c_read(uint8_t bus_num, unsigned int chip_address, uint8_t reg,int addr_offset_len,
                                            uint8_t *read_value, uint8_t dlen)
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    int8_t i2c_status = 0;
    int ret = 0;

#ifdef CONFIG_DM_I2C
        struct udevice *dev,*bus;
#endif

#ifdef CONFIG_DM_I2C
        i2c_status = eBBTF_utils_set_i2c_bus(bus_num);
#else
        i2c_status = i2c_set_bus_num(bus_num);
#endif

    if (i2c_status != 0) {
        struct DiagEventParams_s event;
        Event_Init( 0, &event);
        event.eventType = DIAG_BUS_NOT_FOUND_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = bus_num;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

#ifdef CONFIG_DM_I2C
        i2c_status = eBBTF_utils_get_i2c_bus(&bus);
        if (i2c_status)
		return DIAG_TEST_RESULT_FAIL;
        ret = i2c_get_chip(bus, chip_address, addr_offset_len, &dev);
        if (!ret)
                ret = i2c_set_chip_offset_len(dev, addr_offset_len);
        if (!ret)
                ret = dm_i2c_read(dev, reg, read_value, dlen);
#else
    	ret = i2c_read(chip_address, reg, addr_offset_len, read_value, dlen);
#endif

    if (ret != 0) {
        struct DiagEventParams_s event;
        Event_Init( 0, &event);
        event.eventType = DIAG_DEV_READ_ERR_EVENT;
        event.addressDim = 3;
        event.address[0] = bus_num;
        event.address[1] = chip_address;
        event.address[2] = reg;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    return status;
}

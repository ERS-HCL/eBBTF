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
#include "test_i2cprobe.h"
#include <stdbool.h>

TestResultTypes_e I2C_interface_test(uint32_t testId,void * arg )
{
    int32_t probe_status = 0;
    uint8_t dev_id = 0;
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    int32_t bus_status = 0;
    bool bDeviceFound = false;
    uint8_t chip;
    A_I2C_interface_test *a = (A_I2C_interface_test *)arg;

    if (NULL == arg) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

#ifdef CONFIG_DM_I2C
                bus_status = eBBTF_utils_set_i2c_bus(a->bus_no);
#else
                bus_status = i2c_set_bus_num(a->bus_no);
#endif

    if (bus_status < 0) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_BUS_INIT_ERR_EVENT;
        event.addressDim  = 1;
        event.address[0] = a->bus_no;
        ReportEvent( &event);

        return DIAG_TEST_RESULT_FAIL;
    }

    for (dev_id = 0; dev_id < MAX_I2C_DEVICE; dev_id++) {
        bDeviceFound = false;
        for(chip = 0; chip < a->no_of_device; ++chip) {
            if(dev_id == a->dev_address_list[chip]){
                bDeviceFound = true;
                break;
            }
        }

#ifdef CONFIG_DM_I2C
                struct udevice *bus, *dev;

        	if (eBBTF_utils_get_i2c_bus(&bus))
                	return CMD_RET_FAILURE;

                probe_status = dm_i2c_probe(bus, dev_id, 0, &dev);
#else
                probe_status = i2c_probe(dev_id);
#endif


        if ((probe_status == 0) && (bDeviceFound == false)) {
            struct DiagEventParams_s event;
            Event_Init( testId, &event);
            event.eventType = DIAG_DEV_UNEXPECTED_DEVICE_ERR_EVENT;
            event.addressDim = 2;
            event.address[0] = a->bus_no;
            event.address[1] = dev_id;
            ReportEvent( &event);

            status = DIAG_TEST_RESULT_FAIL;
        } else if ((probe_status != 0) && (bDeviceFound == true)) {
            struct DiagEventParams_s event;
            Event_Init( testId, &event);
            event.eventType = DIAG_DEV_NOT_PRESENT_ERR_EVENT;
            event.addressDim = 2;
            event.address[0] = a->bus_no;
            event.address[1] = dev_id;
            sprintf( event.string, "%s", a->dev_name_list[chip]);
            ReportEvent( &event);

            status = DIAG_TEST_RESULT_FAIL;
        }
    }
    return status;
}


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
#include "test_w25q32_norflash.h"

#include <common.h>
#include <div64.h>
#include <dm.h>
#include <malloc.h>
#include <mapmem.h>
#include <spi.h>
#include <spi_flash.h>
#include <jffs2/jffs2.h>
#include <linux/mtd/mtd.h>

#include <asm/io.h>
#include <dm/device-internal.h>

#include <stdbool.h>
#include <linux/delay.h>


static struct spi_flash *flash;

unsigned char data_test_buf1[SPI_TEST_LEN] = {0};
unsigned char data_test_buf2[SPI_TEST_LEN] = {0};

void spi_pin_mux_cfg(void)
{
     /* Configure for SPI mode */
     writel(SPI0_CLK_MODE0, SPI0_CLK_OFFSET);
     writel(SPI0_D0_MODE0, SPI0_D0_OFFSET);
     writel(SPI0_D1_MODE0, SPI0_D1_OFFSET);
     writel(SPI0_CS0_MODE0, SPI0_CS0_OFFSET);
}

int spi_flash_erase_read_write(int spi_offset, int spi_len)
{
        int ret;

        /* Consistency checking */
        if (spi_offset + spi_len > flash->size) {
                printf("            ERROR: attempting past flash size (%#x)\n",
                       flash->size);
                return -1;
        }

        ret = spi_flash_read(flash, spi_offset, spi_len, data_test_buf1);
        printf("            SF: Read  %#x bytes from %#x : %s\n", (size_t)spi_len, (u32)spi_offset,
               ret ? "ERROR" : "OK");

	if(ret)
            return -1;

        ret = spi_flash_erase(flash, spi_offset, spi_len);
        printf("            SF: Erase %#x bytes from %#x : %s\n", (size_t)spi_len, (u32)spi_offset,
               ret ? "ERROR" : "OK");

        if(ret)
            return -1;

        ret = spi_flash_write(flash, spi_offset, spi_len, data_test_buf1);
        printf("            SF: Write %#x bytes from %#x : %s\n", (size_t)spi_len, (u32)spi_offset,
               ret ? "ERROR" : "OK");

        if(ret)
            return -1;

	ret = spi_flash_read(flash, spi_offset, spi_len, data_test_buf2);
        printf("            SF: Read  %#x bytes from %#x : %s\n", (size_t)spi_len, (u32)spi_offset,
               ret ? "ERROR" : "OK");

#if 1
	ret = spi_flash_erase(flash, spi_offset, 0x400000);
        printf("            SF: Erase %#x bytes from %#x---- : %s\n", (size_t)spi_len, (u32)spi_offset,
               ret ? "ERROR" : "OK");
#endif
        if(ret)
            return -1;

	if(memcmp(data_test_buf1, data_test_buf2, SPI_TEST_LEN)) {
           return -1;	
	}

	return 0;
}

int spi_nor_init(int spi_bus_no, int spi_cs_no)
{
        unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
        unsigned int mode = CONFIG_SF_DEFAULT_MODE;
#ifdef CONFIG_DM_SPI_FLASH
        struct udevice *new, *bus_dev;
        int ret;
        /* In DM mode defaults will be taken from DT */
        speed = 0, mode = 0;
#else
        struct spi_flash *new;
#endif

	#ifdef CONFIG_DM_SPI_FLASH
        /* Remove the old device, otherwise probe will just be a nop */
        ret = spi_find_bus_and_cs(spi_bus_no, spi_cs_no, &bus_dev, &new);
        if (!ret) {
                device_remove(new, DM_REMOVE_NORMAL);
        }
        flash = NULL;
	printf("            "); /* Output align */
        ret = spi_flash_probe_bus_cs(spi_bus_no, spi_cs_no, speed, mode, &new);
        if (ret) {
                printf("Failed to initialize SPI flash at %u:%u (error %d)\n",
                       spi_bus_no, spi_cs_no, ret);
                return -1;
        }

        flash = dev_get_uclass_priv(new);
#else
        if (flash)
                spi_flash_free(flash);

        new = spi_flash_probe(spi_bus_no, spi_cs_no, speed, mode);
        flash = new;

        if (!new) {
                printf("Failed to initialize SPI flash at %u:%u\n", 
				 spi_bus_no, spi_cs_no);
                return -1;
        }

        flash = new;
#endif
        return 0;
}

TestResultTypes_e NOR_Flash_test(uint32_t testId,void * arg )
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_NOR_Flash_test *a = (A_NOR_Flash_test *)arg;

    if (NULL == arg) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

    if(a->spi_bus_no == 0) {
       spi_pin_mux_cfg();
    }
    else {
       printf("Bus number not supported \n");
       return DIAG_TEST_RESULT_FAIL;
    }

    if( spi_nor_init(a->spi_bus_no, a->spi_cs_no) < 0 ) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_BUS_INIT_ERR_EVENT;
        event.addressDim  = 1;
        event.address[0] = a->spi_bus_no;
        ReportEvent( &event);

	//Restore
	spi_pin_mux_cfg();
        return DIAG_TEST_RESULT_FAIL;
    }

    if( spi_flash_erase_read_write(a->spi_offset, a->spi_len) < 0 ) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_BUS_INIT_ERR_EVENT;
        event.addressDim  = 1;
        event.address[0] = a->spi_bus_no;
        ReportEvent( &event);
	spi_pin_mux_cfg();
        return DIAG_TEST_RESULT_FAIL;
    }

    printf("\n");
    return status; 
}

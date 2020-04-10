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

#include "../../testsuite/test_ping.h"
INVOKE_TEST_WITH_PARAM( bcm2836_bbb, "Ping", ping_test, smsc95xx_eth) = {
   "10.144.172.135"
};




#include "../../testsuite/test_mmc.h"
#define RPI2_MMC_BASE_ADDR       0x3f300000

INVOKE_TEST_WITH_PARAM(bcm2836_rpi2, "eMMC presence", MMC_Detection_Test, eMMC_check) = {
        RPI2_MMC_BASE_ADDR   /* eMMC device */
};

#define EMMC_TEST_BLOCK_START     583 
#define EMMC_TEST_BLOCK_CNT       16 
#define EMMC_TEST_DEVICE          0

INVOKE_TEST_WITH_PARAM(bcm2836_rpi2, "eMMC Read, Write and Erase tests",
                MMC_WRITE_READ_Test, eMMC_Access_Check) = {
        RPI2_MMC_BASE_ADDR,   /* eMMC device */
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


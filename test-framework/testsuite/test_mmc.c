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
#include "../common/process_diagtest_cmds.h"
#include "test_mmc.h"
#include <mmc.h>
#include <malloc.h>
#include <fsl_esdhc.h>
#include <cli.h>

#define MAX_SDHC_CONTROLLERS        16
#define MAX_BUFF_SIZE               2048
#define MMC_WRITE_PATTERN_1         0xAA
#define MMC_WRITE_PATTERN_2         0x55

#define MAX_SDHC_CONTROLLERS        16
#define MAX_BUFF_SIZE               2048
#define MMC_WRITE_PATTERN_1         0xAA
#define MMC_WRITE_PATTERN_2         0x55
#define MMC_MAX_TEST_BLOCK          0x10


struct mmc_param_s{
    int8_t curr_device;
    uint32_t block_start;
    uint32_t block_cnt;
    uint32_t dev_id;
    struct mmc *pmmc;
};
struct mmc_param_s ommc_param;

#define MMC_BUFFER_SIZE    8192

char test_buffer_1[MMC_BUFFER_SIZE] = {0};
char test_buffer_2[MMC_BUFFER_SIZE] = {0};

TestResultTypes_e MMC_Dev_Init(uint32_t testId, uint32_t mmc_port_address,
                                                struct mmc_param_s *pmmc_param)
{
    uint8_t dev_id             = 0;
    bool    dev_id_found       = false;
    int32_t mmc_init_status    = 0;
    struct  fsl_esdhc_cfg *cfg = NULL;

    /* find dev_id of SDHC port with base address pointed by "mmc_port_address" */
    for(dev_id = 0; dev_id < MAX_SDHC_CONTROLLERS; dev_id++) {
        pmmc_param->pmmc = find_mmc_device(dev_id);
        if (NULL == pmmc_param->pmmc) {
           /* ID not assigned to any SDHC port;check the next ID */
            continue;
        }

        /* Base address verification */
        cfg = pmmc_param->pmmc->priv;
        if (cfg->esdhc_base == mmc_port_address) {
            pmmc_param->curr_device = dev_id;
		printf("dev found\n");
            dev_id_found = true;
            break;
        }
    }
    if (dev_id_found == false) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_NOT_PRESENT_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = mmc_port_address;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    pmmc_param->pmmc->has_init = 0;
    mmc_init_status = mmc_init(pmmc_param->pmmc);

    if (mmc_init_status < 0) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_INIT_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = mmc_port_address;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }
    {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_INFO_EVENT;
        event.addressDim = 2;
        event.address[0] = mmc_port_address;
        event.address[1] = dev_id;
        sprintf ( event.string, "%s detected on mmc[%d]",
                IS_SD(pmmc_param->pmmc) ? "SD Card" : "eMMC flash", pmmc_param->curr_device);
        ReportEvent( &event);
    }
    return DIAG_TEST_RESULT_PASS;
}



TestResultTypes_e MMC_Detection_Test(uint32_t testId, void *arg)
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_MMC_Detection_Test *a = (A_MMC_Detection_Test *)arg;
    // Safety check for arguments being passed
    if (NULL == a) {
       DEBUG_ASSERT();
       return DIAG_TEST_RESULT_ABORTED;
    }
     status = MMC_Dev_Init(testId, a->base_address, &ommc_param);
    return status;
}

TestResultTypes_e MMC_Read_Blocks(struct mmc_param_s *mmc_parameters, char *data_buffer)
{
        u32 n;
        int32_t mmc_init_status  = 0;

        mmc_parameters->pmmc = find_mmc_device(mmc_parameters->dev_id);
        if (NULL == mmc_parameters->pmmc) {
           /* ID not assigned to any SDHC port;check the next ID */
           return DIAG_TEST_RESULT_FAIL;
        }

        mmc_parameters->pmmc->has_init = 0;
        mmc_init_status = mmc_init(mmc_parameters->pmmc);

        if(mmc_init_status < 0) {
            return DIAG_TEST_RESULT_FAIL;
        }

        printf("\nMMC read: dev # %d, block # %d, count %d ... ",
               mmc_parameters->curr_device, mmc_parameters->block_start,
               mmc_parameters->block_cnt);

        n = blk_dread(mmc_get_blk_desc(mmc_parameters->pmmc), mmc_parameters->block_start,
                                mmc_parameters->block_cnt, data_buffer);

        printf("%d blocks read: %s\n", n, (n == mmc_parameters->block_cnt)
                                          ? "OK" : "ERROR");

        return (n == mmc_parameters->block_cnt)
                ? DIAG_TEST_RESULT_PASS : DIAG_TEST_RESULT_FAIL;
}

TestResultTypes_e MMC_Write_Blocks(struct mmc_param_s *mmc_parameters, char *data_buffer)
{
        u32 n;
        int32_t mmc_init_status  = 0;

        mmc_parameters->pmmc = find_mmc_device(mmc_parameters->dev_id);
        if (NULL == mmc_parameters->pmmc) {
           /* ID not assigned to any SDHC port;check the next ID */
           return DIAG_TEST_RESULT_FAIL;
        }

        if(mmc_init_status < 0) {
            return DIAG_TEST_RESULT_FAIL;
        }

        printf("\nMMC write: dev # %d, block # %d, count %d ... ",
               mmc_parameters->curr_device, mmc_parameters->block_start,
               mmc_parameters->block_cnt);

        n = blk_dwrite(mmc_get_blk_desc(mmc_parameters->pmmc), mmc_parameters->block_start,
                                mmc_parameters->block_cnt, data_buffer);

        printf("%d blocks write: %s\n", n, (n == mmc_parameters->block_cnt) ?
                                           "OK" : "ERROR");

        return (n == mmc_parameters->block_cnt)
                ? DIAG_TEST_RESULT_PASS : DIAG_TEST_RESULT_FAIL;
}

TestResultTypes_e MMC_Erase_Blocks(struct mmc_param_s *mmc_parameters)
{
        u32 n;
        int32_t mmc_init_status  = 0;

        mmc_parameters->pmmc = find_mmc_device(mmc_parameters->dev_id);
        if (NULL == mmc_parameters->pmmc) {
           /* ID not assigned to any SDHC port;check the next ID */
           return DIAG_TEST_RESULT_FAIL;
        }

        mmc_parameters->pmmc->has_init = 0;
        mmc_init_status = mmc_init(mmc_parameters->pmmc);

        if(mmc_init_status < 0) {
            return DIAG_TEST_RESULT_FAIL;
        }

        printf("\nMMC erase: dev # %d, block # %d, count %d ... ",
               mmc_parameters->curr_device, mmc_parameters->block_start,
               mmc_parameters->block_cnt );

        if (mmc_getwp(mmc_parameters->pmmc) == 1) {
                printf("Error: card is write protected!\n");
                return DIAG_TEST_RESULT_FAIL;
        }
        n = blk_derase(mmc_get_blk_desc(mmc_parameters->pmmc), mmc_parameters->block_start,
                                mmc_parameters->block_cnt);

        printf("%d blocks erased: %s\n", n, (n == mmc_parameters->block_cnt ) ?
                                            "OK" : "ERROR");

        return (n == mmc_parameters->block_cnt) ? DIAG_TEST_RESULT_PASS : DIAG_TEST_RESULT_FAIL;
}

TestResultTypes_e MMC_WRITE_READ_Test(uint32_t testId, void *arg)
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_MMC_WRITE_READ_Test *a = (A_MMC_WRITE_READ_Test *)arg;

    if (NULL == a) {
       DEBUG_ASSERT();
       return DIAG_TEST_RESULT_ABORTED;
    }
    else {
       return DIAG_TEST_RESULT_PASS;
    }

    if(a->block_cnt > MMC_MAX_TEST_BLOCK) {
      return DIAG_TEST_RESULT_ABORTED;
    }

    ommc_param.block_start = a->block_start;
    ommc_param.block_cnt = a->block_cnt;
    ommc_param.dev_id = a->mmc_test_dev;


    printf("\n");
    printf("          Starting from Block  %d \n",a->block_start);
    printf("          Number of blocks     %d \n",a->block_cnt);

    status = MMC_Read_Blocks(&ommc_param, test_buffer_1);
    if(status == DIAG_TEST_RESULT_FAIL) {
       struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_VALUE_NOT_EXPECTED_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = a->base_address;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
	}

    status = MMC_Erase_Blocks(&ommc_param);
    if(status == DIAG_TEST_RESULT_FAIL) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_INIT_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = a->base_address;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    status = MMC_Write_Blocks(&ommc_param, test_buffer_1);
    if(status == DIAG_TEST_RESULT_FAIL) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_INIT_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = a->base_address;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }


    status = MMC_Read_Blocks(&ommc_param, test_buffer_2);
    if(status == DIAG_TEST_RESULT_FAIL) {
        struct DiagEventParams_s event;
        Event_Init( testId, &event);
        event.eventType = DIAG_DEV_INIT_ERR_EVENT;
        event.addressDim = 1;
        event.address[0] = a->base_address;
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }
	
	if(memcmp(test_buffer_1, test_buffer_2, (ommc_param.block_cnt * 512 /*Block size */))) 
	{
        	status = DIAG_TEST_RESULT_FAIL;
    	}else 
	{
        	status = DIAG_TEST_RESULT_PASS;
    	}

    return status;
}


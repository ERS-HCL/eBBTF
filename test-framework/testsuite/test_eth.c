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
#include "test_eth.h"

#include <stdbool.h>
#include <miiphy.h>
#include <linux/mii.h>
#include <linux/delay.h>

TestResultTypes_e Ethernet_test(uint32_t testId, void *arg )
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_Ethernet_test *a = (A_Ethernet_test *)arg;
    unsigned int bmcr = 0;
    unsigned short an_advert_val = 0;
    unsigned short an_lp_val = 0;
    unsigned short bm_sts_val = 0;

    int d_cnt;

    if (NULL == arg) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

    if(strcmp(a->phy_name, "LAN8710A")) {
	    return DIAG_TEST_RESULT_ABORTED;
    }

    printf("\n                    Checks 10/100 Full/half duplex with Autonegotiation enabled\n\n");

    miiphy_read(a->dev_name, a->phy_addr, MII_BMSR, &bm_sts_val);

    if(!(bm_sts_val & BMSR_LSTATUS)) 
	{
        
		printf("                    No Link !!!!!!!!!!!\n"); 
        	struct DiagEventParams_s event;
        	Event_Init( testId, &event);
        	event.eventType = DIAG_DEV_NOT_PRESENT_ERR_EVENT;
        	event.addressDim = 0;
        	ReportEvent( &event);
        	return DIAG_TEST_RESULT_FAIL;
    }

		
	
    printf("                    Starting 10 Mbps and Half duplex test ");
    
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, 0x0000);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, MII_BMCR_RESET);
    miiphy_write(a->dev_name, a->phy_addr, MII_ANAR, ADVERTISE_10HALF);

    bmcr |= (MII_BMCR_AN_ENA | MII_BMCR_RESTART);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, bmcr);

    for(d_cnt = 0; d_cnt < 200; d_cnt++) 
        mdelay(10);

    miiphy_read(a->dev_name, a->phy_addr, MII_BMSR, &bm_sts_val);

    if(bm_sts_val & BMSR_ANEGCOMPLETE) {
       miiphy_read(a->dev_name, a->phy_addr, MII_ANAR, &an_advert_val);
       miiphy_read(a->dev_name, a->phy_addr, MII_LPA, &an_lp_val);

       if( (an_lp_val & an_advert_val) & ADVERTISE_10HALF) {
           printf("..... Passed\n"); 
       }
       else {
           printf("..... Link Partner mismatch\n");
       } 
    }
    else {
           printf("..... Auto Negotiation Pending\n");
    }

    printf("                    Starting 10 Mbps and Full duplex test ");
   
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, 0x0000);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, MII_BMCR_RESET);
    miiphy_write(a->dev_name, a->phy_addr, MII_ANAR, ADVERTISE_10FULL);

    bmcr |= (MII_BMCR_AN_ENA | MII_BMCR_RESTART);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, bmcr);

    for(d_cnt = 0; d_cnt < 200; d_cnt++)
        mdelay(10);

    miiphy_read(a->dev_name, a->phy_addr, MII_BMSR, &bm_sts_val);

    if(bm_sts_val & BMSR_ANEGCOMPLETE) {
       miiphy_read(a->dev_name, a->phy_addr, MII_ANAR, &an_advert_val);
       miiphy_read(a->dev_name, a->phy_addr, MII_LPA, &an_lp_val);

       if( (an_lp_val & an_advert_val) & ADVERTISE_10FULL) {
           printf("..... Passed\n");
       }
       else {
           printf("..... Link Partner mismatch \n");
		struct DiagEventParams_s event;
                Event_Init( testId, &event);
                event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                event.addressDim = 0;
                ReportEvent( &event);
                return DIAG_TEST_RESULT_FAIL;
       }
 
    }
    else {
           printf("..... Auto Negotiation Pending\n");
    }

    printf("                    Starting 100 Mbps and Half duplex test ");
  
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, 0x0000);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, MII_BMCR_RESET);
    miiphy_write(a->dev_name, a->phy_addr, MII_ANAR, ADVERTISE_100HALF);

    bmcr |= (MII_BMCR_AN_ENA | MII_BMCR_RESTART);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, bmcr);

    for(d_cnt = 0; d_cnt < 200; d_cnt++)
        mdelay(10);

    miiphy_read(a->dev_name, a->phy_addr, MII_BMSR, &bm_sts_val);

    if(bm_sts_val & BMSR_ANEGCOMPLETE) {
       miiphy_read(a->dev_name, a->phy_addr, MII_ANAR, &an_advert_val);
       miiphy_read(a->dev_name, a->phy_addr, MII_LPA, &an_lp_val);

       if( (an_lp_val & an_advert_val) & ADVERTISE_100HALF) {
           printf("..... Passed\n");
       }
       else {
           printf("..... Link Partner mismatch\n");
		struct DiagEventParams_s event;
                Event_Init( testId, &event);
                event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                event.addressDim = 0;
                ReportEvent( &event);
                return DIAG_TEST_RESULT_FAIL;

       }
    }
    else {
           printf("..... Auto Negotiation Pending\n");
    }


    printf("                    Starting 100 Mbps and Full duplex test ");
 
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, 0x0000);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, MII_BMCR_RESET);
    miiphy_write(a->dev_name, a->phy_addr, MII_ANAR, ADVERTISE_100FULL);

    bmcr |= (MII_BMCR_AN_ENA | MII_BMCR_RESTART);
    miiphy_write(a->dev_name, a->phy_addr, MII_BMCR, bmcr);

    for(d_cnt = 0; d_cnt < 200; d_cnt++)
        mdelay(10);

    miiphy_read(a->dev_name, a->phy_addr, MII_BMSR, &bm_sts_val);

    if(bm_sts_val & BMSR_ANEGCOMPLETE) {
       miiphy_read(a->dev_name, a->phy_addr, MII_ANAR, &an_advert_val);
       miiphy_read(a->dev_name, a->phy_addr, MII_LPA, &an_lp_val);

       if( (an_lp_val & an_advert_val) & ADVERTISE_100FULL) {
           printf("..... Passed\n");
       }
       else {
           printf("..... Link Partner mismatch\n");
		struct DiagEventParams_s event;
                Event_Init( testId, &event);
                event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                event.addressDim = 0;
                ReportEvent( &event);
                return DIAG_TEST_RESULT_FAIL;

       }
    }
    else {
           printf("..... Auto Negotiation Pending\n");
    }

    printf("\n");
    return status; 
}

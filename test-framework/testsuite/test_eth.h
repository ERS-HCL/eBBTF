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
#ifndef TEST_ETH_H
#define TEST_ETH_H

#include <common.h>
#include "../common/process_diagtest_cmds.h"

#define MII_ANAR   0x04
#define MII_BMCR   0x00
#define MII_BMSR   0x01

/* Basic mode control register. */
#define MII_BMCR_FULLDPLX               0x0100  /* Full duplex                 */
#define MII_BMCR_ANRESTART              0x0200  /* Auto negotiation restart    */
#define MII_BMCR_ISOLATE                0x0400  /* Isolate data paths from MII */
#define MII_BMCR_PDOWN                  0x0800  /* Enable low power state      */
#define MII_BMCR_AN_ENA                 0x1000  /* Enable auto negotiation     */
#define MII_BMCR_100MBIT                0x2000  /* Select 100Mbps              */
#define MII_BMCR_LOOPBACK               0x4000  /* TXD loopback bits           */
#define MII_BMCR_RESET                  0x8000  /* Reset to default state      */
#define MII_BMCR_RESTART                0x0200  /* AN restart */
#define MII_BMCR_10MBIT                 0x0000  /* Select 10Mbps               */
#define MII_BMCR_DUPLEX                 0x0100  /* Full duplex */

/* Basic mode status  register. */
#define MII_BMSR_100HALF2               0x0200  /* Can do 100BASE-T2 HDX       */
#define MII_BMSR_100FULL2               0x0400  /* Can do 100BASE-T2 FDX       */
#define MII_BMSR_10HALF                 0x0800  /* Can do 10mbps, half-duplex  */
#define MII_BMSR_10FULL                 0x1000  /* Can do 10mbps, full-duplex  */
#define MII_BMSR_100HALF                0x2000  /* Can do 100mbps, half-duplex */
#define MII_BMSR_100FULL                0x4000  /* Can do 100mbps, full-duplex */
#define MII_BMSR_100BASE4               0x8000  /* Can do 100mbps, 4k packets  */


/**
 * ETH Railings_Test
 *
 * The test configures and verifies various speed-duplex combinations
 *
 */

TestResultTypes_e Ethernet_test( uint32_t testId, void * arg);

#define MAX_NAME_LEN 64

typedef struct {
    char dev_name[MAX_NAME_LEN];
    uint8_t phy_addr;
    char phy_name[MAX_NAME_LEN];
}A_Ethernet_test;

#endif

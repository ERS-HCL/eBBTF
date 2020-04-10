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
#include "test_ping.h"

TestResultTypes_e
ping_test(uint32_t testId, void *arg)
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_ping_test *a = (A_ping_test*) arg;
    struct DiagEventParams_s event;

    Event_Init( testId, &event);


    /* Global variables update */
    net_ping_ip = string_to_ip(a->ip_address);
    if (net_ping_ip.s_addr == 0) {
        event.eventType = DIAG_DEV_CONFIG_ERR_EVENT;
        sprintf ( event.string, "Invalid IP address");
        ReportEvent( &event);
        return DIAG_TEST_RESULT_FAIL;
    }

    int pingResult = -1;

        pingResult = net_loop(PING);

    if (pingResult < 0) {
        event.eventType = DIAG_DEV_DATA_TIMEOUT_ERR_EVENT;
        sprintf( event.string, "ping to %s failed",a->ip_address);
        ReportEvent( &event);

        status = DIAG_TEST_RESULT_FAIL;
    }

    return status;
}

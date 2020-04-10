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


#ifndef EVENT_ID_H
#define EVENT_ID_H

#include "diagTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {DIAG_DIM_ADDRESS_SET_EVENT = 6};
enum {DIAG_DIM_STRING_EVENT = 256};

typedef enum {

    DIAG_INFO_EVENT                   = 2,

    DIAG_GENERIC_ERR_EVENT,
    DIAG_ASSERT_ERR_EVENT,
    DIAG_COM_ASSERT_ERR_EVENT,
    DIAG_TEST_NOT_FOUND_ERR_EVENT,

    DIAG_BUS_INIT_ERR_EVENT,
    DIAG_BUS_READ_ERR_EVENT,
    DIAG_BUS_WRITE_ERR_EVENT,
    DIAG_BUS_NOT_FOUND_ERR_EVENT,
    DIAG_BUS_BUFFER_OVERRUN_ERR_EVENT,
    DIAG_BUS_BUFFER_UNDERRUN_ERR_EVENT,
    DIAG_BUS_CONFIG_ERR_EVENT,
    DIAG_BUS_DATA_MISMATCH_ERR_EVENT,
    DIAG_BUS_TIMEOUT_ERR_EVENT,

    DIAG_DEV_INIT_ERR_EVENT,
    DIAG_DEV_NOT_PRESENT_ERR_EVENT,
    DIAG_DEV_READ_ERR_EVENT,
    DIAG_DEV_WRITE_ERR_EVENT,
    DIAG_DEV_DATA_MISMATCH_ERR_EVENT,
    DIAG_DEV_DATA_TIMEOUT_ERR_EVENT,
    DIAG_DEV_COMMAND_ERR_EVENT,
    DIAG_DEV_ID_MISMATCH_ERR_EVENT,
    DIAG_DEV_ADDR_ERR_EVENT,
    DIAG_DEV_RESET_ERR_EVENT,
    DIAG_DEV_CONFIG_ERR_EVENT,
    DIAG_DEV_BUFFER_OVERRUN_ERR_EVENT,
    DIAG_DEV_BUFFER_UNDERRUN_ERR_EVENT,
    DIAG_DEV_UNEXPECTED_DEVICE_ERR_EVENT,
    DIAG_DEV_TIMEOUT_ERR_EVENT,
    DIAG_DEV_ERASE_ERR_EVENT,
    DIAG_VALUE_NOT_EXPECTED_ERR_EVENT,
    DIAG_VALUE_TOO_SMALL_ERR_EVENT,
    DIAG_VALUE_TOO_LARGE_ERR_EVENT,
    DIAG_VALUE_NOT_IN_RANGE_ERR_EVENT,
    DIAG_MODE_PROTECTED_ERR_EVENT,
    DIAG_MEMORY_ALLOC_ERR_EVENT,
    DIAG_MEMORY_FREE_ERR_EVENT,
} DiagErrEventIDTypes_e;

typedef enum
{
    DIAG_CONTROLRUN_IDLE        = 0,
    DIAG_CONTROLRUN_START,
    DIAG_CONTROLRUN_STOP,
    DIAG_CONTROLRUN_ABORT,
} ControlRunTypes_e;

typedef enum
{
    DIAG_TEST_RESULT_NOT_STARTED      = 0,
    DIAG_TEST_RESULT_IN_PROGRESS,
    DIAG_TEST_RESULT_PASS,
    DIAG_TEST_RESULT_ABORTED,
    DIAG_TEST_RESULT_TIMEOUT,
    DIAG_TEST_RESULT_FAIL,
} TestResultTypes_e;


struct DiagEventParams_s
{
    DiagErrEventIDTypes_e eventType; /* Event type */
    uint32_t sequence;
    uint32_t tv_sec;   
    uint32_t tv_nsec;
    uint32_t objectId;
    uint32_t addressDim;
    uint32_t address[DIAG_DIM_ADDRESS_SET_EVENT];
    char     string[DIAG_DIM_STRING_EVENT];
};


#ifdef __cplusplus
}
#endif

#endif

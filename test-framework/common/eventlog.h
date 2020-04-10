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

#ifndef LOG_H
#define LOG_H

#include "UbootTypes.h"
#include "eventId.h"


typedef enum {
    CONSOLE_LOG_MODE,
}LogMode_e;

void SetLoggingMode( LogMode_e logMode);

void Event_Init( uint32_t testId, struct DiagEventParams_s * event);

void ReportEvent( struct DiagEventParams_s * event);

void ReportErrEvent( uint32_t testId, DiagErrEventIDTypes_e error_code);

void ReportResult( uint32_t testId, TestResultTypes_e result);

void ReportErrEventMsg (uint32_t tid, DiagErrEventIDTypes_e error_code, const char * fmt, ...);

/**
 * A message to indicate an error occurred.
 */
#define DEBUG_ASSERT() ReportErrEventMsg(0, DIAG_ASSERT_ERR_EVENT, "### ASSERT ### %s  %d\n", __FILE__, __LINE__);

#endif

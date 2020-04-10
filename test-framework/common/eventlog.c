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

#include "log.h"
#include "process_diagtest_cmds.h"
#include "consolelog.h"
#include <common.h>
#include <stdarg.h>

static uint32_t curTestId = 0;
static LogMode_e s_logMode = CONSOLE_LOG_MODE;


void SetLoggingMode( LogMode_e logMode)
{
    s_logMode = logMode;
}

void ReportResult( uint32_t testId, TestResultTypes_e result)
{
    curTestId = 0;

    if (CONSOLE_LOG_MODE == s_logMode) {
        console_showresult( testId, result);
    }
}

void Event_Init( uint32_t testId, struct DiagEventParams_s * event)
{
    if( testId == 0 )
        testId = curTestId;

    memset(event, 0, sizeof(*event));
    event->objectId = testId;
}

void ReportEvent( struct DiagEventParams_s * event)
{
    if (CONSOLE_LOG_MODE == s_logMode) {
        console_showevent( event);
    }

}

void ReportErrEventMsg( uint32_t testId, DiagErrEventIDTypes_e eventType, const char * fmt, ...)
{
    va_list vl;
    va_start( vl, fmt);

    if (CONSOLE_LOG_MODE == s_logMode) { 
        console_showerroreventmessage( testId, eventType, fmt, vl);
    }

    va_end(vl);
}


void ReportErrEvent( uint32_t testId, DiagErrEventIDTypes_e eventType)
{
    if (CONSOLE_LOG_MODE == s_logMode) {
        console_showerrevent( testId, eventType);
    }

}


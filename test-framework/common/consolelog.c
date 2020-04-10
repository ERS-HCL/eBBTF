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
#include "consolelog.h"
#include "process_diagtest_cmds.h"

#include <common.h>
#include <stdarg.h>


static LOG_MASK s_logMask = LOG_MASK_ALL;

void SetLogMask( LOG_MASK lm )
{
    s_logMask = lm;
}

LOG_MASK GetLogMask (void)
{
    return s_logMask;
}

char * GetEventName( DiagErrEventIDTypes_e event)
{

#define PROCESS_EVENT_ID( x ) case x: return &(#x[0]);
    switch (event) {

        PROCESS_EVENT_ID( DIAG_INFO_EVENT );

        PROCESS_EVENT_ID( DIAG_GENERIC_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_ASSERT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_COM_ASSERT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_TEST_NOT_FOUND_ERR_EVENT );

        /* Bus error codes */
        PROCESS_EVENT_ID( DIAG_BUS_INIT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_READ_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_WRITE_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_NOT_FOUND_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_BUFFER_OVERRUN_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_BUFFER_UNDERRUN_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_CONFIG_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_DATA_MISMATCH_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_BUS_TIMEOUT_ERR_EVENT );

        /* Device error codes */
        PROCESS_EVENT_ID( DIAG_DEV_INIT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_NOT_PRESENT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_READ_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_WRITE_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_DATA_MISMATCH_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_DATA_TIMEOUT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_COMMAND_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_ID_MISMATCH_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_ADDR_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_RESET_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_CONFIG_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_BUFFER_OVERRUN_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_BUFFER_UNDERRUN_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_UNEXPECTED_DEVICE_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_TIMEOUT_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_DEV_ERASE_ERR_EVENT )
        /* Test error codes - error not related to hardware */
        PROCESS_EVENT_ID( DIAG_VALUE_NOT_EXPECTED_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_VALUE_TOO_SMALL_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_VALUE_TOO_LARGE_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_VALUE_NOT_IN_RANGE_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_MODE_PROTECTED_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_MEMORY_ALLOC_ERR_EVENT );
        PROCESS_EVENT_ID( DIAG_MEMORY_FREE_ERR_EVENT );

    }
    {
        static char tempStatusName[32];

        memset( tempStatusName, 0, sizeof(tempStatusName));
        sprintf (tempStatusName, "Status_%03d ", (int)event);
        return tempStatusName;
    }
}

char * GetTestName( uint32_t testId)
{
    testParamEntry_s * testEntry = LocateTest( testId);

    if (testEntry)
        return testEntry->name;
    {
        static char tempTestName[32];

        memset( tempTestName, 0, sizeof(tempTestName));
        sprintf (tempTestName, "Test_%03d ", (int)testId);
        return tempTestName;
    }
}

char * GetTestResult( TestResultTypes_e result)
{

#define CASE_ENUM( x ) case x: return &(#x[10]);
    switch (result) {
    CASE_ENUM( DIAG_TEST_RESULT_NOT_STARTED);
    CASE_ENUM( DIAG_TEST_RESULT_IN_PROGRESS);
    CASE_ENUM( DIAG_TEST_RESULT_PASS);
    CASE_ENUM( DIAG_TEST_RESULT_ABORTED);
    CASE_ENUM( DIAG_TEST_RESULT_TIMEOUT);
    CASE_ENUM( DIAG_TEST_RESULT_FAIL);
    }
    {
        static char tempResultName[32];

        memset( tempResultName, 0, sizeof(tempResultName));
        sprintf (tempResultName, "Result_%10d ", (int)result);
        return tempResultName;
    }
}

void console_showevent( struct DiagEventParams_s * event)
{
    printf( "  %08X  Evt: %s\n", event->objectId, GetEventName( event->eventType));
    if (event->addressDim) {
        printf( "  %08X       Address", event->objectId);
        int index;
        for (index=0; index<event->addressDim; ++index) {
            printf( " - %X", event->address[index]);
        }
        printf("\n");
    }
    if (event->string) {
        printf( "  %08X       %s\n", event->objectId, event->string);
    }
}

void console_showresult( uint32_t testId, TestResultTypes_e result)
{
    if (DIAG_TEST_RESULT_PASS == result && !(s_logMask & LOG_MASK_PASS))
        return;
    printf( "  %08X  Res: %s\n", testId, GetTestResult(result));
}

void console_showerrevent( uint32_t testId, uint32_t error_code)
{
    if (!(s_logMask & LOG_MASK_ERROR))
        return;

    printf( "  %08X  Err: %s\n", testId, GetEventName(error_code));
}

void console_showerroreventmessage( uint32_t testId, DiagErrEventIDTypes_e error_code, const char * fmt, va_list vl)
{
    if (!(s_logMask & LOG_MASK_ERROR))
        return;

    printf( "  %08X  Emg: %s ", testId, GetEventName(error_code));
    vprintf( fmt, vl );
    printf( "\n" );
}


void console_showinfo( uint32_t testId, const char * fmt, va_list vl)
{
    if (!(s_logMask & LOG_MASK_INFO))
        return;

    printf( "  %08X  Inf: ", testId);
    vprintf( fmt, vl );
    printf( "\n" );
}


void console_showdebuginfo( uint32_t testId, const char * fmt, va_list vl)
{
    if (!(s_logMask & LOG_MASK_ERROR))
        return;

    printf( "  %08X  Dbg: ", testId);
    vprintf( fmt, vl );
    printf( "\n" );
}

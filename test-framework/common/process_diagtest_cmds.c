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
#include "process_diagtest_cmds.h"
#include "generateTestId.h"
#include "consolelog.h"
#include <common.h>

// Note: BOARD_BOARD_DEFAULT is defined in the UBoot Makefile
#define BCM2836_RPI2 0
#define AM335X_BBB  1


testParamEntry_s * testList_g = 0;
uint32_t       testListDim_g = 0;
const char *   testNameList_g = 0;
static uint32_t exitOnErr_g = 0;
static uint32_t repeatCnt_g = 0;

void ProcessTestList( uint32_t board_index)
{
    switch (board_index)
    {
    default:
        testList_g = 0;
        testListDim_g = 0;
        break;

#define caseSetTestList(board) \
        testList_g = ll_entry_start(testParamEntry_s, testList_##board); \
        testListDim_g = ll_entry_count(testParamEntry_s, testList_##board); \
        testNameList_g = #board

         case BCM2836_RPI2:
                caseSetTestList(bcm2836_rpi2);
                break;

        case AM335X_BBB:
                caseSetTestList(am335x_bbb);
                break;


        }

	if (testList_g) {

        uint32_t testIndex;
        uint32_t calculatedTestId;
        testParamEntry_s * testEntry;

        for (testIndex=0; testIndex<testListDim_g; ++testIndex) {
            testEntry = &testList_g[testIndex];
            calculatedTestId = CalculateTestIdHash( testEntry->name );
            testEntry->testId = calculatedTestId;
            testEntry->status.runCount = 0;
            testEntry->status.totalFailCount = 0;
            testEntry->status.newFailCount = 0;
            testEntry->status.ResultFromSingleRun = DIAG_TEST_RESULT_NOT_STARTED;
            testEntry->status.ResultFromLoopedRun = DIAG_TEST_RESULT_NOT_STARTED;
            testEntry->status.bInProgress = 0;
            testEntry->request.prevcontrol = DIAG_CONTROLRUN_IDLE;
            testEntry->request.control = DIAG_CONTROLRUN_IDLE;
        }
    }
    else {
        printf("Test list %d not found\n", board_index);
    }
}




testParamEntry_s * RequestTestList( void )
{
    if (0 == testList_g) {
        ProcessTestList( BOARD_DEFAULT);
    }
    return testList_g;
}
	

unsigned long RequestTestListDim( void )
{
    if (0 == testList_g) {
        ProcessTestList( BOARD_DEFAULT);
    }
    return testListDim_g;
}

const char * RequestTestListName( void )
{
    if (0 == testList_g) {
        ProcessTestList( BOARD_DEFAULT);
    }

    if (0 == testNameList_g)
        return "not found";

    return testNameList_g;
}


void ProcessTestLoopCount( uint32_t value)
{
    repeatCnt_g = value;
}

uint32_t RequestTestLoopCount()
{
    return repeatCnt_g;
}

void ProcessExitOnErr( uint32_t value)
{
    exitOnErr_g = value;
}

uint32_t RequestExitOnErr()
{
    return exitOnErr_g;
}

void ClearFailureCounts(void)
{
    uint32_t testIndex;
    uint32_t testListDim = RequestTestListDim();
    testParamEntry_s * testEntry = 0;

    for (testIndex = 0; testIndex < testListDim; ++testIndex)
    {
        testEntry = &RequestTestList()[testIndex];
        testEntry->status.newFailCount = 0;
    }


}

void ExecuteAllTestListed( testList_s * options)
{
    uint32_t loopCount;
    uint32_t testIndex;
    TestResultTypes_e result = DIAG_TEST_RESULT_IN_PROGRESS;

   SetLoggingMode( CONSOLE_LOG_MODE);

    if (0 == options) {
        DEBUG_ASSERT();
        return;
    }

    SetLogMask( options->logLevel );

    if (0 == options->testIdCount) {
        DEBUG_ASSERT();
        return;
    }

    if (options->testIdCount > DIM_TEST_LIST) {
        DEBUG_ASSERT();
        return;
    }

    ClearFailureCounts();

    for (loopCount = 0; loopCount <= options->loopCnt; ++loopCount)
    {
        for (testIndex = 0; testIndex < options->testIdCount; ++testIndex)
        {
            uint32_t testId = options->testIdList[testIndex];
            result = RunDeviceTest( testId);
            if (DIAG_TEST_RESULT_PASS != result) {
                if (options->exitOnErr) {
                    return;
                }
            }
        }
    }
    return;
}

void GetAvailableListOfTest( testList_s * options)
{
    uint32_t testIndex;
    uint32_t testListDim = RequestTestListDim();
    testParamEntry_s * testEntry = 0;

    if (0 == options) {
        DEBUG_ASSERT();
        return;
    }

    //Check whether the test list is larger than receiving list
    if (testListDim > sizeof(options->testIdList)/ sizeof(options->testIdList[0]) ) {
        DEBUG_ASSERT();
        return;
    }

    for (testIndex = 0; testIndex < testListDim; ++testIndex)
    {
        testEntry = &RequestTestList()[testIndex];

        options->testIdList[testIndex] = testEntry->testId;
    }

    options->testIdCount = testListDim;

    return;
}



testParamEntry_s * LocateTest( uint32_t searchTestId )
{
    uint32_t testIndex;
    uint32_t testListDim = RequestTestListDim();
    testParamEntry_s * testEntry = 0;

    for (testIndex = 0; testIndex < testListDim; ++testIndex)
    {
        testEntry = &RequestTestList()[testIndex];
        if (testEntry->testId == searchTestId) {
            // Test Found
            return testEntry;
        }
    }

    // Test not found
    testEntry = 0;
    return testEntry;
}


TestResultTypes_e RunDeviceTest( uint32_t testId )
{
    testParamEntry_s * testEntry = LocateTest( testId);

    if (!testEntry) {
        // Test not found
        ReportErrEvent( testId, DIAG_TEST_NOT_FOUND_ERR_EVENT);
        ReportResult( testId, DIAG_TEST_RESULT_ABORTED);
        return DIAG_TEST_RESULT_ABORTED;
    }

    // Test Found
    if (0 == testEntry->testFunction) {
        DEBUG_ASSERT();
        ReportResult( testId, DIAG_TEST_RESULT_ABORTED);
        return DIAG_TEST_RESULT_ABORTED;
    }

    testEntry->status.runCount++;
    printf( "%08X -- %s -- %s\n", (int)testId, testEntry->description, testEntry->name);
    testEntry->status.ResultFromSingleRun = testEntry->testFunction( testId, testEntry->testArgs);
    if (DIAG_TEST_RESULT_PASS != testEntry->status.ResultFromSingleRun) {

    	testEntry->status.totalFailCount++;
    	testEntry->status.newFailCount++;
    }
    ReportResult( testId, testEntry->status.ResultFromSingleRun );

    return testEntry->status.ResultFromSingleRun;
}

void DisplayTestList( testList_s * options, int showIndex, int showFlags)
{
    uint32_t testIndex;
    testParamEntry_s * testEntry;

    // NULL list
    if (0 == options->testIdCount) {
        DEBUG_ASSERT();
        return;
    }

    // List overflow
    if (options->testIdCount > DIM_TEST_LIST) {
        DEBUG_ASSERT();
        return;
    }
    if (showFlags && options->exitOnErr)
        printf( "    Exit\n");
    if (showFlags && options->loopCnt)
        printf( "    repeat %d times\n", options->loopCnt);


    for (testIndex = 0; testIndex < options->testIdCount; ++testIndex)
    {
        uint32_t calculatedTestId = 0;
        uint32_t testId = options->testIdList[testIndex];

        testEntry = LocateTest( testId );
        if (!testEntry) {
            if (showIndex)
                printf( "%4d -- ", testIndex);
            printf( "%08X -- Error: Test not found\n", (int)testId);
            continue;
        }

        if (showIndex)
		printf( "%4d -- ", testIndex);

        printf( "%08X -- %s -- %s\n", (int)testId, testEntry->description, testEntry->name);
        calculatedTestId = CalculateTestIdHash( testEntry->name );
        if (calculatedTestId != testId) {
            printf ("%20sError: TestId is 0x%08X but calculated 0x%08X\n", "", (uint32_t)testEntry->testId, calculatedTestId );
        }
        {
            uint32_t foundCount = 0;
            uint32_t searchIndex;
            uint32_t searchListDim = RequestTestListDim();
            testParamEntry_s * searchList = RequestTestList();

            for (searchIndex = 0; searchIndex < searchListDim; ++searchIndex)
            {
                if (searchList[searchIndex].testId == testId) {
                    ++foundCount;
                    if (foundCount > 1) {
                        printf ("%20sError: %s has the same TestId\n", "", searchList[searchIndex].name );
                    }
                }
            }
        }
    }
    return;
}



#define INDENT_1 "    "
#define INDENT_2 "           - "

void DisplayTestResults( testList_s * options)
{
    uint32_t testIndex;
    testParamEntry_s * testEntry;

    // NULL list
    if (0 == options->testIdCount) {
        DEBUG_ASSERT();
        return;
    }

    // List overflow
    if (options->testIdCount > DIM_TEST_LIST) {
        DEBUG_ASSERT();
        return;
    }

    for (testIndex = 0; testIndex < options->testIdCount; ++testIndex)
    {
        uint32_t testId = options->testIdList[testIndex];
        uint32_t runCount = 0;
        uint32_t newFailCount = 0;
        uint32_t totalFailCount = 0;

        testEntry = LocateTest( testId );
        if (!testEntry) {
            printf( "*** Error: Unknown test\n");
            printf( INDENT_1 "TestId - %08X\n", (int)testId );
            printf( INDENT_1 "Not found\n");
            continue;
        }

        runCount = testEntry->status.runCount;
        newFailCount = testEntry->status.newFailCount;
        totalFailCount = testEntry->status.totalFailCount;

	/* skip prints for 'pass' */
        if (!(options->logLevel & LOG_MASK_PASS) && !totalFailCount)
        {
            continue;
        }

        printf( INDENT_1 "Name   - %s\n", testEntry->name );
        printf( INDENT_1 "Desc   - %s\n", testEntry->description );
        printf (INDENT_1 "TestId - %08X\n", (uint32_t) testId );
        if (0 == runCount) {
            printf( INDENT_2 "Not run\n");
        }
        else if (1 == runCount) {
            if (0 == totalFailCount) {
                printf( INDENT_2 "Passed\n");
            }
            else {
                printf( INDENT_2 "Failed\n");
            }
        }
        else {
            if (0 == totalFailCount) {
                printf(INDENT_2 "Passed %d times\n", runCount);
            }
            else if (runCount == totalFailCount) {
                printf(INDENT_2 "Failed %d times\n", totalFailCount);
            }
            else if (0 == newFailCount) {
                printf(INDENT_2 "Failed %d of %d times\n", totalFailCount, runCount);
            }
            else if (1 == newFailCount){
                printf(INDENT_2 "Failed %d of %d times. (%d new failure)\n", totalFailCount, runCount, newFailCount);
            }
            else {
                printf(INDENT_2 "Failed %d of %d times. (%d new failures)\n", totalFailCount, runCount, newFailCount);
            }
        }
    }
    return;
}


	
	

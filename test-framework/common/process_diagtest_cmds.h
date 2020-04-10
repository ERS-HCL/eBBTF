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
#ifndef TEST_H
#define TEST_H

#include "UbootTypes.h"
#include "eventlog.h"
#include <common.h>
#include <linker_lists.h>
#include "eventId.h"

// The dimension of a the test list.  This is the max test that can be run by a single request
enum { DIM_TEST_LIST = 200 };

typedef struct 
{
    uint32_t testIdCount;
    uint32_t testIdList[DIM_TEST_LIST];
    uint32_t loopCnt;
    int32_t  exitOnErr;
    int32_t  logLevel;

} testList_s;


void ExecuteAllTestListed( testList_s * options);
void GetAvailableListOfTest( testList_s * options);
void DisplayTestList( testList_s * options, int showIndex, int showFlags);
void DisplayTestResults( testList_s * options);
TestResultTypes_e RunDeviceTest( uint32_t testId );

/**
 * The prototype for all u-boot tests.
 *
 * It accepts a testId that it uses for any logging messages.
 *
 */
typedef TestResultTypes_e TestFunction( uint32_t testId, void * arg);


typedef struct {
    /// test description
    char *         description; //test description
    char *         name;        //test name
    TestFunction * testFunction; //test function
    void *         testArgs;    //holds configuration parameters
    unsigned long  testId;      //Test name
    struct {
        ControlRunTypes_e prevcontrol;
        ControlRunTypes_e control;
    } request;
    struct {
        unsigned long runCount;
        unsigned long totalFailCount;
        unsigned long newFailCount;
        TestResultTypes_e    ResultFromSingleRun;
        TestResultTypes_e    ResultFromLoopedRun;
        int           bInProgress;
    } status;
} testParamEntry_s;


void ProcessTestLoopCount( uint32_t value);
uint32_t RequestTestLoopCount(void);
void ProcessExitOnErr( uint32_t value);
uint32_t RequestExitOnErr(void);

void ProcessTestLoopCount( uint32_t value);
uint32_t RequestTestLoopCount(void);
void ProcessExitOnErr( uint32_t value);
uint32_t RequestExitOnErr(void);

testParamEntry_s * RequestTestList( void );

unsigned long RequestTestListDim( void );

const char * RequestTestListName( void );

void ProcessTestList( uint32_t board_index);

testParamEntry_s * LocateTest( uint32_t testId );

#define INVOKE_TEST_WITH_PARAM_ENTRY( board, description, function, argument) \
    { description, #function"_"#argument, function, &a_##board##_##function##_##argument, 0 }

#define INVOKE_TEST_WITH_ADDITIONAL_PARAM( board, description, function, argument) \
        ll_entry_declare( testParamEntry_s, function##_##argument, testList_##board) = \
        INVOKE_TEST_WITH_PARAM_ENTRY( board, description, function, argument)

#define INVOKE_TEST_WITH_PARAM( board, description, function, argument) \
        extern A_##function a_##board##_##function##_##argument; \
        INVOKE_TEST_WITH_ADDITIONAL_PARAM( board, description, function, argument); \
        A_##function a_##board##_##function##_##argument

#endif

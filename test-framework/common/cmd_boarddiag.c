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

#include <common.h>
#include <command.h>
#include "process_diagtest_cmds.h"
#include "generateTestId.h"
#include "consolelog.h"

static void show_test_list( void) 
{
    testList_s tstlst_obj;

    printf( "List available tests for %s\n", RequestTestListName());

    memset( &tstlst_obj, 0, sizeof(tstlst_obj));
    GetAvailableListOfTest( &tstlst_obj );

    tstlst_obj.logLevel = LOG_MASK_ALL;
    DisplayTestList( &tstlst_obj, 1, 0 );
}

static void run_all_tests( uint32_t exitOnErr, uint32_t loopCnt, uint32_t logLevel ) 
{
    testList_s tstlst_obj;

    memset( &tstlst_obj, 0, sizeof(tstlst_obj));
    GetAvailableListOfTest( &tstlst_obj );

    tstlst_obj.exitOnErr = exitOnErr;
    tstlst_obj.loopCnt = loopCnt;
    tstlst_obj.logLevel = logLevel;

    if (tstlst_obj.logLevel & LOG_MASK_LIST) {
       DisplayTestList( &tstlst_obj, 1, 1 );
       printf("\n");
    }


    ExecuteAllTestListed( &tstlst_obj );
    printf("\n");
    DisplayTestResults( &tstlst_obj );
}

static int runtestByTestId( uint32_t count, uint32_t testlistIndex[], uint32_t exitOnErr, uint32_t loopCnt, uint32_t logLevel ) 
{
    int retval = 0;
    testList_s tstlst_obj;
    testList_s tstlstlookup_obj;
    int index = 0;
    int testIndex;

    memset( &tstlstlookup_obj, 0, sizeof(tstlstlookup_obj));
    GetAvailableListOfTest( &tstlstlookup_obj );

    memset( &tstlst_obj, 0, sizeof(tstlst_obj));
    for (index=0; index<count; ++index) {
        testIndex = testlistIndex[index];
        if (testIndex >= tstlstlookup_obj.testIdCount) {
            printf( "Test %d out of range\n", testIndex);
            return CMD_RET_USAGE;
        }
        tstlst_obj.testIdList[tstlst_obj.testIdCount++] = tstlstlookup_obj.testIdList[testIndex];
    }
    tstlst_obj.exitOnErr = exitOnErr;
    tstlst_obj.loopCnt = loopCnt;
    tstlst_obj.logLevel = logLevel;

    if (tstlst_obj.logLevel & LOG_MASK_LIST) {
       printf( "Run the following tests\n");
       DisplayTestList( &tstlst_obj, 0, 1 );
       printf("\n");
    }
    ExecuteAllTestListed( &tstlst_obj );
    printf("\n");
    DisplayTestResults( &tstlst_obj );

    return retval;
}

static int toLower(int value)
{
    if ('A' <= value && value <= 'Z')
        return value + 'a' - 'A';
    return value;
}

static int isSubCmd(char * cmd, int minMatch, char * arg)
{
    int i;

    for (i=0; i<256; ++i) {
        if (0 == arg[i]) {
            // End of string
            if (i >= minMatch) {
                // matched
                return 1;
            }
            // To short of a match
            return 0;
        }
        else if (toLower(arg[i]) != toLower(cmd[i])) {  //  case insensitive match
            // Not matching
            return 0;
        }
    }
    return 1;
}

/**
 * Increments the CmdArgIndex then gets the next arg.
 *
 * returns a pointer to the next argument or returns NULL point if there is not one
 */
static char * ProcessNextArg(int *pCmdArgIndex, int CmdArgCount, char * const argv[]) {
    int CmdArgIndex = ++(*pCmdArgIndex);

    if (CmdArgIndex >= CmdArgCount)
        return 0;

    return argv[CmdArgIndex];
}

/**
 * Increments the CmdArgIndex then gets the next arg.
 *
 * returns int zero if OK or CMD_RET_USAGE if there is not one.
 */
static int ProcessNextCmdArg(int *pCmdArgIndex, int CmdArgCount, char * const argv[], uint32_t *val) {
    int CmdArgIndex = ++(*pCmdArgIndex);

    if (CmdArgIndex >= CmdArgCount)
        return CMD_RET_USAGE;

    *val = simple_strtoul (argv[CmdArgIndex], NULL, 10);
	
    return 0;
}

static int do_boarddiag (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
    int retval = 0;
    uint32_t exitOnErr = 0;
    uint32_t logLevel = LOG_MASK_ALL;
    uint32_t loopCnt = 0;
    int CmdArgIndex = 0;
    char * nextCmdArg = 0;

    printf ("\n   === Board Diagnostics Software ===\n\n");

    while (1) {
        nextCmdArg = ProcessNextArg( &CmdArgIndex, argc, argv );


        if (!nextCmdArg) {
            return CMD_RET_USAGE;
        }
       
 	 else if (isSubCmd("list", 1, nextCmdArg)) {
            show_test_list();
            return retval;
        }
        else if (isSubCmd("runAll", 4, nextCmdArg)) {

		if(argc == 4)
		{
			nextCmdArg = ProcessNextArg( &CmdArgIndex, argc, argv );
			if (!nextCmdArg) 
            			return CMD_RET_USAGE;
        		
		
			retval = ProcessNextCmdArg( &CmdArgIndex, argc, argv, &loopCnt );
			if (retval)
                		return retval;
                }


            	run_all_tests( exitOnErr, loopCnt, logLevel);
            	return retval;
        }
        else if (isSubCmd("run", 2, nextCmdArg)) 
	{

            uint32_t count = 0;
            uint32_t testIdList[100];

            if (argc <= CmdArgIndex)
	    {
                return CMD_RET_USAGE;
            }

	    if(argc == 3)
	    {
		retval = ProcessNextCmdArg( &CmdArgIndex, argc, argv, &testIdList[count++] );
                if (retval)
                    return retval;

            	if (!count)
                {
                	return CMD_RET_USAGE;
                }


	    }else
	    {
		return CMD_RET_USAGE;
            }

            return runtestByTestId( count, testIdList, exitOnErr, loopCnt, logLevel );
        }
        else {
            printf(" Invalid option %s\n", nextCmdArg);
            return CMD_RET_USAGE;
        }
    }

    return CMD_RET_USAGE;
}


U_BOOT_CMD(boarddiag, CONFIG_SYS_MAXARGS, 0, do_boarddiag, "Board diagnostics Software",
        "[args...]\n"
        "   list           -- List available device tests\n"
	"   run <test_num> -- Run individual test by test ID specified in the list\n"
        "   runAll         -- Run all tests\n"
        "          loop <times> -- Set loop to execute a test 'n' number of times. \n"
		);


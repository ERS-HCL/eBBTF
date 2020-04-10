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
#include "test_memory.h"
#include <asm/cache.h>

// data bus test
// this test writes a patter (bit representation of powers of two)
// to a specific address, flushes cache, then reads the data at
// the address to check if it is still what was written

TestResultTypes_e memory_data_line_test( uint32_t testId, void * arg )
{
	uint32_t pat;
    	uint32_t read;

        A_memory_data_line_test * a = (A_memory_data_line_test *)arg;
        struct DiagEventParams_s event;
        Event_Init( testId, &event);

	volatile uint32_t * addr = (uint32_t *)a->mem_address;


        // Safety check for arguments being passed
        if (0 == a) {
                DEBUG_ASSERT();
                return DIAG_TEST_RESULT_ABORTED;
        }

        TestResultTypes_e status = DIAG_TEST_RESULT_PASS;

    	for (pat = 1; pat != 0; pat <<= 1)
    	{
        	*addr = pat;

        	//flush dcache
        	flush_dcache_all();

        	read = *addr;
        	if (read != pat) 
		{
			event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                        event.addressDim = 3;
                        event.address[0] = a->mem_address;
                        event.address[1] = pat;
                        event.address[2] = read;
                        ReportEvent( &event );

                        status = DIAG_TEST_RESULT_FAIL;

                        break;

        	}
    	}

	return status;
}

TestResultTypes_e memory_address_line_test( uint32_t testId, void * arg )
{
       	uint32_t mask, offsetA, offsetB;
    	// two patterns to test the effects of flipping 1s to 0s and flipping 0s to 1s
    	uint32_t patA = 0xAAAAAAAA;
    	uint32_t patB = ~patA;

        A_memory_address_line_test * a = (A_memory_address_line_test *)arg;
        struct DiagEventParams_s event;
        Event_Init( testId, &event);

        volatile uint32_t * addr = (uint32_t *)a->mem_address;
	uint32_t length = (uint32_t)a->size;


        // Safety check for arguments being passed
        if (0 == a) {
                DEBUG_ASSERT();
                return DIAG_TEST_RESULT_ABORTED;
        }

        TestResultTypes_e status = DIAG_TEST_RESULT_PASS;

	// write pattern A to addresses offset by powers of two
	// keep doing so until length amount is written
    	mask = length/sizeof(uint32_t) -1;
    	for (offsetA = 1; (offsetA & mask); offsetA <<= 1) {
        	addr[offsetA] = patA;
    	}

    	offsetB = 0;
    	addr[offsetB] = patB;

    	//flush dcache
    	flush_dcache_all();

	// loop back through the addresses and confirm that the
    	// contents are what was written initially. Upon finding the first
    	// error, return
    	for (offsetA = 1; (offsetA & mask); offsetA <<= 1) 
	{
        	if (addr[offsetA] != patA) 
		{
			event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                	event.addressDim = 2;
                	event.address[0] = patA;
                	event.address[1] = addr[offsetA];;
                	ReportEvent( &event );

                	return DIAG_TEST_RESULT_FAIL;
        	}
    	}

    	addr[offsetB] = patA;

    	// write pattern B to addresses offset by powers of two
    	// keep doing so until length amount is written
    	for (offsetB = 1; (offsetB & mask); offsetB <<= 1) 
    	{
        	addr[offsetB] = patB;

        	//flush dcache
        	flush_dcache_all();

        	// if for some reason writing offsets greater than 1
        	// effect offset 0, then we have a problem (probably a short?)
        	if (addr[0] != patA) 
		{
			event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                        event.addressDim = 3;
			event.address[0] = (uint32_t) &addr[offsetB];
                        event.address[1] = patA;
                        event.address[2] = addr[0];;
                        ReportEvent( &event );

                        return DIAG_TEST_RESULT_FAIL;
        	}

		// loop back through the addresses and confirm that the
        	// contents are what was written initially. Upon finding the first
        	// error, return
        	for (offsetA = 1; (offsetA & mask); offsetA <<= 1) 
		{
            		if ((addr[offsetA] != patA) && (offsetA != offsetB)) 
			{
				event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                        	event.addressDim = 3;
                        	event.address[0] = (uint32_t) &addr[offsetB];
                        	event.address[1] = patA;
                        	event.address[2] = addr[offsetA];
                        	ReportEvent( &event );

                        	return DIAG_TEST_RESULT_FAIL;

            		}
        	}
        	
		addr[offsetB] = patA;
    }
    
	return status;
}

TestResultTypes_e memory_pattern_test( uint32_t testId, void * arg )
{
        A_memory_pattern_test * a = (A_memory_pattern_test *)arg;
        struct DiagEventParams_s event;
        Event_Init( testId, &event);

	// Safety check for arguments being passed
        if (0 == a) {
                DEBUG_ASSERT();
                return DIAG_TEST_RESULT_ABORTED;
        }

        TestResultTypes_e status = DIAG_TEST_RESULT_PASS;

	unsigned long i;
        ulong *mem = (ulong *) (uint32_t *)a->mem_address;
	uint32_t value = a->value;
	uint32_t length = a->size;
	
        ulong readback;

        for (i = 0; i < length / sizeof (ulong); i++)
        {
                mem[i] = (ulong)value;
        }

        for (i = 0; i < length / sizeof (ulong) ; i++)
        {
                readback = mem[i];
                if (readback != (ulong)value)
                {
			event.eventType = DIAG_DEV_DATA_MISMATCH_ERR_EVENT;
                        event.addressDim = 3;
                        event.address[0] = (uint32_t) mem;
			event.address[1] = (uint32_t)value;
                        event.address[2] = readback;
                        ReportEvent( &event );
			status = DIAG_TEST_RESULT_FAIL;
			break;
                }
        }

	return status;

}


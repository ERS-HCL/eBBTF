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
#include "test_gpio.h"

#include <asm/arch/hardware.h>
#include <asm/cache.h>
#include <asm/emif.h>


TestResultTypes_e GPIO_test(uint32_t testId, void *arg)
{
   
    uint32_t reg;
 
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_GPIO_test *a = (A_GPIO_test*) arg;
 
    // Safety check for arguments being passed
    if (0 == a) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }
 
    //Pin Mux
    __raw_writel(a->pad_ctrl_val, ((a->pad_ctrl_base_addr) + (a->pad_ctrl_offset)));	

    //Enable GPIO module
    __raw_writel(a->gpio_bank_enable_val, ((a->gpio_bank_base_addr) + (a->gpio_bank_enable_offset)));

   if(a->gpio_direction == 1)
   {

    	//Set the GPIOs as output
    	reg = __raw_readl(((a->gpio_bank_base_addr) + (a->gpio_bank_output_offset)));
    	__raw_writel((reg & ~(1 << a->gpio_pin)), ((a->gpio_bank_base_addr) + (a->gpio_bank_output_offset)));

	/*Read the pin to verify whether the pin is already enabled */
        reg = __raw_readl(((a->gpio_bank_base_addr) + (a->gpio_bank_datain_offset)));


	if(a->enable)
	{

		if(reg & (1 << a->gpio_pin))
		{
			//Pin is already enabled
                	status = DIAG_TEST_RESULT_FAIL;
			return status;
		}
		 
		/* Enable the GPIOS by writing 1 in DataOut*/
	        __raw_writel((1 << a->gpio_pin) , ((a->gpio_bank_base_addr) + (a->gpio_bank_dataout_offset)));    

        	/*Read back the pin and compare */
        	reg = __raw_readl(((a->gpio_bank_base_addr) + (a->gpio_bank_datain_offset)));

        	if(reg & (1 << a->gpio_pin))
                	status = DIAG_TEST_RESULT_PASS;
        	else
                	status = DIAG_TEST_RESULT_FAIL;
	}else
	{
		/* Disable the GPIOS by writing 1 in DataOut*/
                __raw_writel(~(1 << a->gpio_pin) , ((a->gpio_bank_base_addr) + (a->gpio_bank_dataout_offset)));

                /*Read back the pin and compare */
                reg = __raw_readl(((a->gpio_bank_base_addr) + (a->gpio_bank_datain_offset)));

                if(reg & ~(1 << a->gpio_pin))
                        status = DIAG_TEST_RESULT_PASS;
                else
                        status = DIAG_TEST_RESULT_FAIL;

	}

   }

 
    return status;
}


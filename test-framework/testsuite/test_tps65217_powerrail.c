/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (c) HCL Technologies Limited, 2020
 
 * Author:Titus Dhanasingh  <titus.dhanasingh@hcl.com>
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
#include "test_tps65217_powerrail.h"

#include <stdbool.h>
#include <linux/delay.h>


uint32_t pmic_regulation_regs[PMIC_REGULATION_REGS] = {PMIC_DCDC1_DDR, PMIC_DCDC2_MPU,
                                                       PMIC_DCDC3_CORE, PMIC_LD01,
                                                       PMIC_LD02, PMIC_LD03, PMIC_LD04};


TestResultTypes_e PWR_railings_test(uint32_t testId,void * arg )
{
    TestResultTypes_e status = DIAG_TEST_RESULT_PASS;
    A_PWR_railings_test *a = (A_PWR_railings_test *)arg;
    uint8_t i2c_read_resp = 0;
    uint8_t pwr_ok  = 0;
    uint8_t reg_cnt = 0;
    uint32_t vdd_val = 0;
    char vdd_arr[100] = { }; 

    if (NULL == arg) {
        DEBUG_ASSERT();
        return DIAG_TEST_RESULT_ABORTED;
    }

    if(strcmp(a->dev_name, "TPS65217C")) {
	    return DIAG_TEST_RESULT_ABORTED;
    }

    for(reg_cnt = 0; reg_cnt < PMIC_REGULATION_REGS; reg_cnt++) {


        status = eBBTF_utils_i2c_read(a->i2c_bus_no, a->i2c_dev_addr, pmic_regulation_regs[reg_cnt], 1,&i2c_read_resp, 1);

        if (status == DIAG_TEST_RESULT_FAIL) {
            return status;
        }

	status = eBBTF_utils_i2c_read(a->i2c_bus_no, a->i2c_dev_addr, PMIC_STATUS_FLAGS /*Power, regulation OK */, 1,&pwr_ok, 1);


        if (status == DIAG_TEST_RESULT_FAIL) {
            return status;
        }

	/* TPS65217C PMIC */
        switch (pmic_regulation_regs[reg_cnt]) {

               case PMIC_DCDC1_DDR:
	       case PMIC_DCDC2_MPU:
	       case PMIC_DCDC3_CORE:
               case PMIC_LD02:
               {
                    i2c_read_resp =  i2c_read_resp & 0x0000003f;

                    if(i2c_read_resp <= 0x17) {
                       vdd_val = 900 + (i2c_read_resp * 25);
                    }

                    if(i2c_read_resp > 0x17 && i2c_read_resp <= 0x34) {
                       vdd_val = 1500 + ((i2c_read_resp - 0x18) * 50);
                    }

                    if(i2c_read_resp > 0x34 && i2c_read_resp <= 0x3f) {
                       vdd_val = 3000 + ((i2c_read_resp - 0x35) * 100);
                    }

		    if(vdd_val >= 1000) {
			    sprintf(vdd_arr,"%d.%3d", (vdd_val / 1000),
					             (vdd_val % 1000));
	            } else {
                            sprintf(vdd_arr,"0.3%d", vdd_val);
	            }

                    if( pmic_regulation_regs[reg_cnt] == PMIC_DCDC1_DDR ) {
                        printf("           VDD DDR   - %sV", vdd_arr);
			printf(" Power Regulation - %s \n", (pwr_ok & 0x10)?"OK":"NOT OK !!");
	            } else if( pmic_regulation_regs[reg_cnt] == PMIC_DCDC2_MPU ) {
			printf("           VDD MPU   - %sV", vdd_arr);
			printf(" Power Regulation - %s \n", (pwr_ok & 0x08)?"OK":"NOT OK !!");

	            } else if( pmic_regulation_regs[reg_cnt] == PMIC_DCDC3_CORE ) {
                        printf("           VDD CORE  - %sV", vdd_arr);
			printf(" Power Regulation - %s \n", (pwr_ok & 0x04)?"OK":"NOT OK !!");
                    } else {
                        printf("           LDO2      - %sV",vdd_arr);
			printf(" Power Regulation - %s \n", (pwr_ok & 0x01)?"OK":"NOT OK !!");
                    }

               }
	       break;
	       case PMIC_LD01:
	       {
		   char LDO1_Voltage[16][5] = {"1.0","1.1","1.2","1.25","1.3","1.35","1.4","1.5",
			                        "1.6","1.8","2.5","2.75","2.8","3.0","3.1","3.3"};
		   uint8_t ldo1_val;

		   ldo1_val  =  i2c_read_resp & 0x0000000f;

		   printf("           LD01      - %sV",LDO1_Voltage[ldo1_val]);
		   printf("   Power Regulation - %s \n", (pwr_ok & 0x02)?"OK":"NOT OK !!");

               }
	       break;
	       case PMIC_LD03:
	       case PMIC_LD04: 
	       {
                    char LD0_3_4_val[32][10] = {"1.5V","1.55V","1.6V","1.65V","1.7V","1.75V","1.8V","1.85V",
			                    "1.90V","2V","2.1V","2.2V","2.3V","2.4V","2.45V","2.5V",
					    "2.55V","2.6V","2.65V","2.7V","2.75V","2.8V","2.85V","2.9V",
					    "2.95V","3V","3.05V","3.1V","3.15V","3.2V","3.25V","3.3V"};
                    uint8_t ldo_3_4_val;

		    ldo_3_4_val =  i2c_read_resp & 0x0000001f;

                    if(pmic_regulation_regs[reg_cnt] == PMIC_LD03) {
			     printf("           LDO3      - %s", LD0_3_4_val[ldo_3_4_val]);
			     printf("   Power Regulation - %s \n", (pwr_ok & 0x40)?"OK":"NOT OK !!");
	            } else {
			     printf("           LDO4      - %s ", LD0_3_4_val[ldo_3_4_val]);
			     printf("  Power Regulation - %s \n", (pwr_ok & 0x20)?"OK":"NOT OK !!");
	            }
               }
	       break;
	       default:
	              status = DIAG_TEST_RESULT_FAIL;
       }
   }

   printf("\n");
   return status; 
}


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
#ifndef TEST_GPIO_H
#define TEST_GPIO_H

typedef struct {
    uint32_t pad_ctrl_base_addr;
    uint32_t pad_ctrl_offset;
    uint32_t pad_ctrl_val;
    uint32_t gpio_bank_base_addr;
    uint32_t gpio_bank_enable_offset;
    uint32_t gpio_bank_enable_val;
    uint32_t gpio_direction;
    uint32_t gpio_bank_output_offset;
    uint32_t gpio_bank_datain_offset;
    uint32_t gpio_bank_dataout_offset;
    uint32_t gpio_pin;
    uint32_t enable;

}A_GPIO_test;


TestResultTypes_e GPIO_test(uint32_t testId, void *arg);

#endif /* TEST_GPIO_H */


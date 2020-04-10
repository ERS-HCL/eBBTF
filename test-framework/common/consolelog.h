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
 

#ifndef CONSOLE_LOG_H
#define CONSOLE_LOG_H

#include "UbootTypes.h"
#include "eventId.h"

typedef enum {
    LOG_MASK_PASS  		= 0x01,
    LOG_MASK_ERROR 		= 0x02,
    LOG_MASK_INFO  		= 0x04,
    LOG_MASK_DEBUG 		= 0x08,
    LOG_MASK_STATUS 	= 0x10,
    LOG_MASK_LIST   	= 0x20,
    LOG_MASK_COM   		= 0x40,
    LOG_MASK_COM_ERROR 	= 0x80,

    LOG_MASK_ALL = 0xFFFF,
} LOG_MASK;


void SetLogMask( LOG_MASK lm);

LOG_MASK GetLogMask( void );

void console_showevent( struct DiagEventParams_s * event);
void console_showerrevent( uint32_t testId, DiagErrEventIDTypes_e error_code);

/**
 * test status.
 */
void console_showresult( uint32_t testId, TestResultTypes_e result);

void console_showerroreventmessage( uint32_t testId, DiagErrEventIDTypes_e error_code, const char * fmt, va_list vl);
void console_showinfo( uint32_t testId, const char * fmt, va_list vl);
void console_showdebuginfo( uint32_t testId, const char * fmt, va_list vl);

#endif

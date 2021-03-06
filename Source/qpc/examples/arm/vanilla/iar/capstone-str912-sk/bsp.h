/*****************************************************************************
* Product: Board Support Package for the STR912-SK Evaluaiton Board
* Last Updated for Version: 4.0.03
* Date of the Last Update:  Mar 31, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

#include "91x_lib.h"                                        /* ST ST91x MCU */

#define BSP_TICKS_PER_SEC       50

void BSP_init(void);
uint32_t BSP_get_ticks(void);
void BSP_LED_on(uint8_t n);
void BSP_LED_off(uint8_t n);
void BSP_LCD_light_on(void);
void BSP_LCD_light_off(void);

#endif                                                             /* bsp_h */



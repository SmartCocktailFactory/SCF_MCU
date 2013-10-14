/*****************************************************************************
* BSP for Explorer 16 board with PIC24FJ128GA010, Vanilla kernel
* Last Updated for Version: 4.0.04
* Date of the Last Update:  May 26, 2009
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

#include <p24FJ128GA010.h>        /* for declaration of the SRBits register */

                                               /* the system tick rate [Hz] */
#define BSP_TICKS_PER_SEC    100UL

void BSP_init(void);
void BSP_displyPhilStat(uint8_t n, char const Q_ROM *stat);
#define BSP_busyDelay()      ((void)0)

#endif                                                             /* bsp_h */

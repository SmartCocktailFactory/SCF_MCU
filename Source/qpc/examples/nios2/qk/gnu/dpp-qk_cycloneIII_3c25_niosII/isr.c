/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.1.02
* Date of the Last Update:  Jan 28, 2010
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2010 Quantum Leaps, LLC. All rights reserved.
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
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

/*..........................................................................*/
void QF_onStartup(void) {
    QF_INT_KEY_TYPE intLockKey;
    QF_INT_LOCK(intLockKey);

    /* register your interrupts */

    QF_INT_UNLOCK(intLockKey);
}

/* add your interrupt service routines (ISRs) here ... */

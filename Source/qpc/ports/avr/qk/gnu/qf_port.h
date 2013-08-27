/*****************************************************************************
* Product:  QF/C port, AVR, GNU compiler
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Dec 10, 2011
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
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
#ifndef qf_port_h
#define qf_port_h

#define QF_MAX_ACTIVE           8
#define QF_MAX_EPOOL            3
#define QF_EVENT_SIZ_SIZE       1
#define QF_EQUEUE_CTR_SIZE      1
#define QF_MPOOL_SIZ_SIZE       1
#define QF_MPOOL_CTR_SIZE       1
#define QF_TIMEEVT_CTR_SIZE     2

                                             /* QF interrupt disable/enable */
#define QF_INT_DISABLE()        cli()
#define QF_INT_ENABLE()         sei()

                                          /* QF critical section entry/exit */
#define QF_CRIT_STAT_TYPE       uint8_t
#define QF_CRIT_ENTRY(stat_)    do { \
    (stat_) = SREG; \
    cli(); \
} while (0)
#define QF_CRIT_EXIT(stat_)     (SREG = (stat_))


#include <avr\io.h>                                      /* SREG definition */
#include <avr\interrupt.h>                                   /* cli()/sei() */

#include "qep_port.h"                                           /* QEP port */
#include "qk_port.h"                                             /* QK port */
#include "qf.h"                 /* QF platform-independent public interface */

#endif                                                         /* qf_port_h */

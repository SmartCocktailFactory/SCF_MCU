/*****************************************************************************
* Product: QDPP example
* Last Updated for Version: 4.2.00
* Date of the Last Update:  Jul 23, 2011
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
#include "qp_port.h"
#include "active_objects/ao_def.h"
#include "bsp.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvent const *l_tableQueueSto[5];
static QEvent const *l_lwIPMgrQueueSto[10];
static QEvent const *l_mgtProtocolHandlerSto[10];
static QEvent const *l_iceMgrSto[5];
static QSubscrList   l_subscrSto[MAX_PUB_SIG];

static union SmallEvents {
    void   *e0;                                       /* minimum event size */
    uint8_t e1[sizeof(QEvent)];
    uint8_t e2[sizeof(Uint8Evt)];
    /* ... other event types to go into this pool */
} l_smlPoolSto[20];                     /* storage for the small event pool */

static union MediumEvents {
    void   *e0;                                       /* minimum event size */
    uint8_t e1[sizeof(TextEvt)];
    uint8_t e2[sizeof(DataEvt)];
    /* ... other event types to go into this pool */
} l_medPoolSto[10];                     /* storage for the medium event pool */

/*..........................................................................*/
int main(void) {

    Table_ctor();               /* instantiate the Table active object */
    LwIPMgr_ctor();             /* instantiate all LwIP-Manager active object */
    MgtProtocolHandler_ctor();  /* instantiate the MgtProtocolHandler active object */
    IceMgr_ctor();              /* instantiate the IceMgr active object */

    BSP_init();                     /* initialize the Board Support Package */

    QF_init();     /* initialize the framework and the underlying RT kernel */

                                                  /* object dictionaries... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_medPoolSto);
    QS_OBJ_DICTIONARY(l_lwIPMgrQueueSto);
    QS_OBJ_DICTIONARY(l_tableQueueSto);
    QS_OBJ_DICTIONARY(l_mgtProtocolHandlerSto);
    QS_OBJ_DICTIONARY(l_iceMgrSto);

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
    QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));

    QActive_start(AO_LwIPMgr, 1,
                  l_lwIPMgrQueueSto, Q_DIM(l_lwIPMgrQueueSto),
                  (void *)0, 0, (QEvent *)0);

    QActive_start(AO_Table, 3,
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 0, (QEvent *)0);

    QActive_start(AO_MgtProtocolHandler, 5,
                  l_mgtProtocolHandlerSto, Q_DIM(l_mgtProtocolHandlerSto),
                  (void *)0, 0, (QEvent *)0);

    QActive_start(AO_IceMgr, 7,
                  l_iceMgrSto, Q_DIM(l_iceMgrSto),
                  (void *)0, 0, (QEvent *)0);

    QF_run();                                     /* run the QF application */

    return 1;
}

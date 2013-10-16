/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.2.00
* Date of the Last Update:  Jul 14, 2011
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
#include "qp_port.h"                                 /* QP port header file */
#include "active_objects/ao_def.h"     /* application events and active objects */
#include "bsp.h"                       /* Board Support Package header file */
#include <stdio.h>
#include "omx_p207_eval/lcd/lcd.h"


Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct TableTag {
    QActive super;
    uint8_t udpCtr;
} Table;

static QState Table_initial(Table *me, QEvent const *e);
static QState Table_serving(Table *me, QEvent const *e);

static void Table_displayInit(Table *me);
static void Table_displayIPAddr(Table *me, char const *ip_addr);
static void Table_displayUdpText(Table *me, char const *text);
static void Table_displayCgiText(Table *me, char const *text);


#ifdef Q_SPY
enum AppRecords {                  /* application-specific QS trace records */
    PHILO_STAT = QS_USER,
    CGI_TEXT,
    UDP_TEXT,
};
#endif

/* Local objects -----------------------------------------------------------*/
static Table l_table;     /* the single instance of the Table active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_Table = (QActive *)&l_table;      /* "opaque" AO pointer */

/*..........................................................................*/
void Table_ctor(void) {
    Table *me = &l_table;

    QActive_ctor(&me->super, (QStateHandler)&Table_initial);

    me->udpCtr = 0;
}
/*..........................................................................*/
QState Table_initial(Table *me, QEvent const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    /* Initialize the OLED display */
    Table_displayInit(me);

    QActive_subscribe((QActive *)me, DONE_SIG);
    QActive_subscribe((QActive *)me, DISPLAY_IPADDR_SIG);
    QActive_subscribe((QActive *)me, DISPLAY_CGI_SIG);
    QActive_subscribe((QActive *)me, DISPLAY_UDP_SIG);

    QS_OBJ_DICTIONARY(&l_table);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&Table_initial);
    QS_FUN_DICTIONARY(&Table_serving);

    QS_SIG_DICTIONARY(DISPLAY_IPADDR_SIG,  0);     /* global signals */
    QS_SIG_DICTIONARY(DISPLAY_CGI_SIG,     0);
    QS_SIG_DICTIONARY(DISPLAY_UDP_SIG,     0);

	  QS_SIG_DICTIONARY(TERMINATE_SIG, 	   0);


    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    return Q_TRAN(&Table_serving);
}
/*..........................................................................*/
QState Table_serving(Table *me, QEvent const *e) {

    switch (e->sig) {
		    case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
        case DISPLAY_IPADDR_SIG: {
            Table_displayIPAddr(me, ((TextEvt *)e)->text);
            return Q_HANDLED();
        }
        case DISPLAY_CGI_SIG: {
            Table_displayCgiText(me, ((TextEvt *)e)->text);
            return Q_HANDLED();
        }
        case DISPLAY_UDP_SIG: {
            TextEvt *te;

            Table_displayUdpText(me, ((TextEvt *)e)->text);
            ++me->udpCtr;

            te = Q_NEW(TextEvt, SEND_UDP_SIG);
            snprintf(te->text, Q_DIM(te->text), "%s-%d",
                     ((TextEvt const *)e)->text, (int)me->udpCtr);
            QACTIVE_POST(AO_LwIPMgr, (QEvent *)te, me);    /* post directly */

            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

/* helper functions for the display ........................................*/
                     
void Table_displayInit(Table *me) {
    /* init LCD display here, if available */
    LCDPutStr("IP:", 110, 5, SMALL, BLACK, WHITE);
    LCDPutStr("CMD:", 80, 5, LARGE, RED, WHITE);
    LCDPutStr("CGI:", 50, 5, LARGE, RED, WHITE);
    LCDPutStr("UDP:", 20, 5, LARGE, BROWN, WHITE);

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }

}

/*..........................................................................*/
static void Table_displayIPAddr(Table *me, char const *ip_addr) {
    LCDPutStr(ip_addr, 110, 30, SMALL, BLACK, WHITE);
}
/*..........................................................................*/
void Table_displayCgiText(Table *me, char const *text) {
    LCDPutStr(text, 50, 40, LARGE, RED, WHITE);

    QS_BEGIN(CGI_TEXT, 0)              /* application-specific record begin */
        QS_STR(text);                                          /* User text */
    QS_END()
}
/*..........................................................................*/
void Table_displayUdpText(Table *me, char const *text) {
    LCDPutStr(text, 20, 40, LARGE, BROWN, WHITE);
    QS_BEGIN(UDP_TEXT, 0)              /* application-specific record begin */
        QS_STR(text);                                          /* User text */
    QS_END()
}



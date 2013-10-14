/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        This module implements the management protocol. This protocol is
                      used to communicate between the Management Component (Multicore)
                      and the microcontroller part.

 ****************************************************************************************

 ****************************************************************************************
 Includes
*****************************************************************************************/
#include "qp_port.h"                                 /* QP port header file */
#include "active_objects/ao_def.h"     /* application events and active objects */
#include "bsp.h"                       /* Board Support Package header file */
#include <stdio.h>
#include "omx_p207_eval/led/led.h"


/****************************************************************************************
 Defines
*****************************************************************************************/


/****************************************************************************************
 Enums
*****************************************************************************************/


/****************************************************************************************
 Structs and typedefs
*****************************************************************************************/
/* Active object class -----------------------------------------------------*/
typedef struct MgtProtocolHandlerTag {
    QActive super;
    uint8_t aoExampleData;   /* not used; define specific AO data here */
} MgtProtocolHandler;


/****************************************************************************************
 Macros
*****************************************************************************************/


/****************************************************************************************
 Forward declarations
*****************************************************************************************/
static QState MgtProtocolHandler_initial(MgtProtocolHandler *me, QEvent const *e);
static QState MgtProtocolHandler_running(MgtProtocolHandler *me, QEvent const *e);


/****************************************************************************************
 Local data
*****************************************************************************************/
Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static MgtProtocolHandler l_mgtProtocolHandler;     /* the single instance of the mgtProtocolHandler active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_MgtProtocolHandler = (QActive *)&l_mgtProtocolHandler;      /* "opaque" AO pointer */


/****************************************************************************************
 Public function implementations
*****************************************************************************************/
/*..........................................................................*/
void MgtProtocolHandler_ctor(void) {
    MgtProtocolHandler *me = &l_mgtProtocolHandler;

    QActive_ctor(&me->super, (QStateHandler)&MgtProtocolHandler_initial);

    me->aoExampleData = 0;
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/
/*..........................................................................*/
QState MgtProtocolHandler_initial(MgtProtocolHandler *me, QEvent const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QActive_subscribe((QActive *)me, PROCESS_UDP_SIG);
    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    QS_OBJ_DICTIONARY(&l_mgtProtocolHandler);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&MgtProtocolHandler_initial);
    QS_FUN_DICTIONARY(&MgtProtocolHandler_running);

    QS_SIG_DICTIONARY(PROCESS_UDP_SIG,     0);
    QS_SIG_DICTIONARY(TERMINATE_SIG,     0);

    return Q_TRAN(&MgtProtocolHandler_running);
}
/*..........................................................................*/
QState MgtProtocolHandler_running(MgtProtocolHandler *me, QEvent const *e) {

    switch (e->sig) {
        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
        case PROCESS_UDP_SIG: {
            /* TODO: process the command received via UDP here */
            omxEval_led_toggle(LED_2);  /* for debugging purposes */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

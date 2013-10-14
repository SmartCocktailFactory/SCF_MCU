/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        This module implements the MOD-IO2 manager.
                      It manages all accesses to the MOD-IO2 extension module
                      (containing relays and GPIO).

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
typedef struct ModIo2MgrTag {
    QActive super;
    uint8_t aoExampleData;   /* not used; define specific AO data here */
} ModIo2Mgr;


/****************************************************************************************
 Macros
*****************************************************************************************/


/****************************************************************************************
 Forward declarations
*****************************************************************************************/
static QState ModIo2Mgr_initial(ModIo2Mgr *me, QEvent const *e);
static QState ModIo2Mgr_running(ModIo2Mgr *me, QEvent const *e);


/****************************************************************************************
 Local data
*****************************************************************************************/
Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static ModIo2Mgr l_modIo2Mgr;     /* the single instance of the modIo2Mgr active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_ModIo2Mgr = (QActive *)&l_modIo2Mgr;      /* "opaque" AO pointer */


/****************************************************************************************
 Public function implementations
*****************************************************************************************/
/*..........................................................................*/
void ModIo2Mgr_ctor(void) {
    ModIo2Mgr *me = &l_modIo2Mgr;

    QActive_ctor(&me->super, (QStateHandler)&ModIo2Mgr_initial);

    me->aoExampleData = 0;
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/
/*..........................................................................*/
static QState ModIo2Mgr_initial(ModIo2Mgr *me, QEvent const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QActive_subscribe((QActive *)me, ENABLE_RELAY_SIG);
    QActive_subscribe((QActive *)me, DISABLE_RELAY_SIG);
    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    QS_OBJ_DICTIONARY(&l_modIo2Mgr);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&ModIo2Mgr_initial);
    QS_FUN_DICTIONARY(&ModIo2Mgr_running);

    QS_SIG_DICTIONARY(ENABLE_RELAY_SIG,  0);
    QS_SIG_DICTIONARY(DISABLE_RELAY_SIG, 0);
    QS_SIG_DICTIONARY(TERMINATE_SIG,     0);

    return Q_TRAN(&ModIo2Mgr_running);
}
/*..........................................................................*/
static QState ModIo2Mgr_running(ModIo2Mgr *me, QEvent const *e) {

    switch (e->sig) {
        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
        case ENABLE_RELAY_SIG: {
            /* TODO: enable the relay here, relay number can be obtained from event data */
            omxEval_led_toggle(LED_4);  /* for debugging purposes */
            return Q_HANDLED();
        }
        case DISABLE_RELAY_SIG: {
            /* TODO: disable the relay here, relay number can be obtained from event data */
            omxEval_led_toggle(LED_4);  /* for debugging purposes */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
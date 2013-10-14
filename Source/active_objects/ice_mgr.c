/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        This module implements the ice manager.
                      The ice manager controls the delivery of ice cubes. It switches
                      the ice cube delivery on and off, considering the number of ice
                      cubes that pass the ice cube sensor.

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
typedef struct IceMgrTag {
    QActive super;
    uint8_t aoExampleData;   /* not used; define specific AO data here */
} IceMgr;


/****************************************************************************************
 Macros
*****************************************************************************************/


/****************************************************************************************
 Forward declarations
*****************************************************************************************/
static QState IceMgr_initial(IceMgr *me, QEvent const *e);
static QState IceMgr_running(IceMgr *me, QEvent const *e);


/****************************************************************************************
 Local data
*****************************************************************************************/
Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static IceMgr l_iceMgr;     /* the single instance of the iceMgr active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_IceMgr = (QActive *)&l_iceMgr;      /* "opaque" AO pointer */


/****************************************************************************************
 Public function implementations
*****************************************************************************************/
/*..........................................................................*/
void IceMgr_ctor(void) {
    IceMgr *me = &l_iceMgr;

    QActive_ctor(&me->super, (QStateHandler)&IceMgr_initial);

    me->aoExampleData = 0;
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/
/*..........................................................................*/
QState IceMgr_initial(IceMgr *me, QEvent const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    QS_OBJ_DICTIONARY(&l_iceMgr);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&IceMgr_initial);
    QS_FUN_DICTIONARY(&IceMgr_running);

    QS_SIG_DICTIONARY(DELIVER_ICE_CUBE_SIG,     0);
    QS_SIG_DICTIONARY(TERMINATE_SIG,     0);

    return Q_TRAN(&IceMgr_running);
}
/*..........................................................................*/
QState IceMgr_running(IceMgr *me, QEvent const *e) {

    switch (e->sig) {
        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
        case DELIVER_ICE_CUBE_SIG: {
            /* TODO: start ice cube delivery process here */
            omxEval_led_toggle(LED_3);  /* for debugging purposes */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}


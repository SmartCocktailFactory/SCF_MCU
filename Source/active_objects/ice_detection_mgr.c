/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        This module implements the ice detection manager.
                      It publishes an event when an ice cube has been detected.

 ****************************************************************************************

 ****************************************************************************************
 Includes
*****************************************************************************************/
#include "qp_port.h"                                 /* QP port header file */
#include "active_objects/ao_def.h"     /* application events and active objects */
#include "bsp.h"                       /* Board Support Package header file */
#include <stdio.h>
#include "omx_p207_eval/ice_detector/ice_detector.h"
#include "omx_p207_eval/led/led.h"


/****************************************************************************************
 Defines
*****************************************************************************************/

#define ICE_DETECTION_TIME   (BSP_TICKS_PER_SEC / 500) /* 2ms */

/****************************************************************************************
 Enums
*****************************************************************************************/


/****************************************************************************************
 Structs and typedefs
*****************************************************************************************/
/* Active object class -----------------------------------------------------*/
typedef struct IceDetectionMgrTag {
    QActive super;
    QTimeEvt timeEvt;
} IceDetectionMgr;

enum InternalSignals {                                  /* internal signals */
    TIMEOUT_SIG = MAX_SIG
};

/****************************************************************************************
 Macros
*****************************************************************************************/


/****************************************************************************************
 Forward declarations
*****************************************************************************************/
static QState IceDetectionMgr_initial(IceDetectionMgr *me, QEvent const *e);
static QState IceDetectionMgr_running(IceDetectionMgr *me, QEvent const *e);
static QState IceDetectionMgr_running_no_ice(IceDetectionMgr * const me, QEvt const * const e);
static QState IceDetectionMgr_running_maybe_ice(IceDetectionMgr * const me, QEvt const * const e);
static QState IceDetectionMgr_running_ice(IceDetectionMgr * const me, QEvt const * const e);


/****************************************************************************************
 Local data
*****************************************************************************************/
Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static IceDetectionMgr l_iceDetectionMgr;     /* the single instance of the iceDetectionMgr active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_IceDetectionMgr = (QActive *)&l_iceDetectionMgr;      /* "opaque" AO pointer */


/****************************************************************************************
 Public function implementations
*****************************************************************************************/
/*..........................................................................*/
void IceDetectionMgr_ctor(void) {
    IceDetectionMgr *me = &l_iceDetectionMgr;

    QActive_ctor(&me->super, (QStateHandler)&IceDetectionMgr_initial);
    QTimeEvt_ctor(&me->timeEvt, TIMEOUT_SIG);
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/
/*..........................................................................*/
static QState IceDetectionMgr_initial(IceDetectionMgr *me, QEvent const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    QS_OBJ_DICTIONARY(&l_iceDetectionMgr);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&IceDetectionMgr_initial);
    QS_FUN_DICTIONARY(&IceDetectionMgr_running);
    QS_FUN_DICTIONARY(&IceDetectionMgr_running_no_ice);
    QS_FUN_DICTIONARY(&IceDetectionMgr_running_maybe_ice);
    QS_FUN_DICTIONARY(&IceDetectionMgr_running_ice);

    QS_SIG_DICTIONARY(TERMINATE_SIG, 0);
    QS_SIG_DICTIONARY(ICE_CUBE_DETECTED_SIG, 0);

    return Q_TRAN(&IceDetectionMgr_running);
}
/*..........................................................................*/
static QState IceDetectionMgr_running(IceDetectionMgr *me, QEvent const *e) {

    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postEvery(&me->timeEvt, (QActive *)me, ICE_DETECTION_TIME);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt);
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {
             return Q_TRAN(&IceDetectionMgr_running_no_ice);
        }

        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

static QState IceDetectionMgr_running_no_ice(IceDetectionMgr * const me, QEvt const * const e) {
  switch (e->sig) {
      case TIMEOUT_SIG: {
          if (omsEval_ice_status() == ICE) {
              return Q_TRAN(&IceDetectionMgr_running_maybe_ice);
          }
          else {
              return Q_HANDLED();
          }
      }
  }
  return Q_SUPER(&IceDetectionMgr_running);
}

static QState IceDetectionMgr_running_maybe_ice(IceDetectionMgr * const me, QEvt const * const e) {
  switch (e->sig) {
      case TIMEOUT_SIG: {
          if (omsEval_ice_status() == ICE) {
              return Q_TRAN(&IceDetectionMgr_running_ice);
          }
          else {
              return Q_TRAN(&IceDetectionMgr_running_no_ice);
          }
      }
  }
  return Q_SUPER(&IceDetectionMgr_running);
}

static QState IceDetectionMgr_running_ice(IceDetectionMgr * const me, QEvt const * const e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
          QF_PUBLISH(Q_NEW(QEvent, ICE_CUBE_DETECTED_SIG), me);
          omxEval_led_on(LED_4);
          return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
          omxEval_led_off(LED_4);
          return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            if (omsEval_ice_status() == NO_ICE) {
                return Q_TRAN(&IceDetectionMgr_running_no_ice);
            }
            else {
                return Q_HANDLED();
            }
        }
    }
    return Q_SUPER(&IceDetectionMgr_running);
}

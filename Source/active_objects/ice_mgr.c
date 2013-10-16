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
#define ICE_DELIVERY_TIMEOUT   (30u*BSP_TICKS_PER_SEC)   /* caution: must fit into uint16_t */


/****************************************************************************************
 Enums
*****************************************************************************************/
enum InternalSignals {                                  /* internal signals */
    TIMEOUT_SIG = MAX_SIG
};


/****************************************************************************************
 Structs and typedefs
*****************************************************************************************/
/* Active object class -----------------------------------------------------*/
typedef struct IceMgrTag {
    QActive super;
    QTimeEvt timeEvt;   /* timeout to stop ice cube delivery */
    uint8_t numberOfIceCubes;
} IceMgr;


/****************************************************************************************
 Macros
*****************************************************************************************/


/****************************************************************************************
 Forward declarations
*****************************************************************************************/
static QState IceMgr_initial(IceMgr *me, QEvent const *e);
static QState IceMgr_stopped(IceMgr *me, QEvent const *e);
static QState IceMgr_delivering(IceMgr *me, QEvent const *e);


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
    QTimeEvt_ctor(&me->timeEvt, TIMEOUT_SIG);
    me->numberOfIceCubes = 0u;
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/
/*..........................................................................*/
static QState IceMgr_initial(IceMgr *me, QEvent const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QActive_subscribe((QActive *)me, TERMINATE_SIG);
    QActive_subscribe((QActive *)me, ICE_CUBE_DETECTED_SIG);

    QS_OBJ_DICTIONARY(&l_iceMgr);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&IceMgr_initial);
    QS_FUN_DICTIONARY(&IceMgr_stopped);
    QS_FUN_DICTIONARY(&IceMgr_delivering);

    QS_SIG_DICTIONARY(DELIVER_ICE_CUBE_SIG, 0);
    QS_SIG_DICTIONARY(TERMINATE_SIG, 0);
    QS_SIG_DICTIONARY(ICE_CUBE_DETECTED_SIG, 0);


    return Q_TRAN(&IceMgr_stopped);
}
/*..........................................................................*/
static QState IceMgr_stopped(IceMgr *me, QEvent const *e) {

    switch (e->sig) {
        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
        case DELIVER_ICE_CUBE_SIG: {
            Uint8Evt *ue = (Uint8Evt *)e;
            if (ue->data > 0u) {
                me->numberOfIceCubes = ue->data;
                return Q_TRAN(&IceMgr_delivering);
            }
            else {
                return Q_HANDLED();
            }
        }
    }
    return Q_SUPER(&QHsm_top);
}

/*..........................................................................*/
static QState IceMgr_delivering(IceMgr *me, QEvent const *e) {

  switch (e->sig) {
      case TERMINATE_SIG: {
          QF_stop();
          return Q_HANDLED();
      }
      case Q_ENTRY_SIG: {
          /* switch relay on */
          Uint8Evt *ue;
          ue = Q_NEW(Uint8Evt, ENABLE_RELAY_SIG);
          ue->data = 1;  /* TODO: use correct relay number here */
          QF_PUBLISH((QEvent *)ue, me);

          /* start timeout */
          QTimeEvt_postIn(&me->timeEvt, (QActive *)me, ICE_DELIVERY_TIMEOUT);
          omxEval_led_on(LED_3);  /* for debugging purposes */
          return Q_HANDLED();
      }
      case Q_EXIT_SIG: {
          /* switch relay off */
          Uint8Evt *ue;
          ue = Q_NEW(Uint8Evt, DISABLE_RELAY_SIG);
          ue->data = 1;  /* TODO: use correct relay number here */
          QF_PUBLISH((QEvent *)ue, me);

          omxEval_led_off(LED_3);  /* for debugging purposes */
          return Q_HANDLED();
      }
      case ICE_CUBE_DETECTED_SIG: {
          me->numberOfIceCubes--;
          if (me->numberOfIceCubes > 0u) {
              return Q_HANDLED();
          }
          else {
              QACTIVE_POST(AO_MgtProtocolHandler, Q_NEW(QEvent, ICE_CUBE_DELIVERY_DONE_SIG), me);
              return Q_TRAN(&IceMgr_stopped);
          }
      }
      case TIMEOUT_SIG: {
          me->numberOfIceCubes = 0u;
          QACTIVE_POST(AO_MgtProtocolHandler, Q_NEW(QEvent, ICE_CUBE_DELIVERY_TMO_SIG), me);
          return Q_TRAN(&IceMgr_stopped);
      }
  }
  return Q_SUPER(&QHsm_top);
}

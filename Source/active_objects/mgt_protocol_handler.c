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
#include "omx_p207_eval/lcd/lcd.h"
#include "base/utils/utils.h"


/****************************************************************************************
 Defines
*****************************************************************************************/
#define MGT_PROTOCOL_CMD_NUMBER_SIZE   4u       /**< Size of command number field in frame */

#define MGT_PROTOCOL_CMD_DELIVER_ICE_CUBE   0x03u    /**< Number of DeliverIceCube command */

#define COMMAND_NUMBER_STRING_SIZE  11u

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
static void mgtProtocolHandler_processFrame(MgtProtocolHandler *me, const uint8_t *frameContent, uint32_t frameLength);
static void mgtProtocolHandler_processDeliverIceCube(MgtProtocolHandler *me, const uint8_t *payload, uint32_t payloadLength);


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
static QState MgtProtocolHandler_initial(MgtProtocolHandler *me, QEvent const *e) {
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
static QState MgtProtocolHandler_running(MgtProtocolHandler *me, QEvent const *e) {

    switch (e->sig) {
        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
        case PROCESS_UDP_SIG: {
            /* TODO: process the command received via UDP here */
            mgtProtocolHandler_processFrame(me, ((DataEvt *)e)->dataBuffer, ((DataEvt *)e)->usedDataBufferLength);
            omxEval_led_toggle(LED_2);  /* for debugging purposes */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

/**
 * Process the frame received from the Management Component.
 *
 * @param me the pointer to the current active object
 * @param frameContent the content of the received frame
 * @param frameLength the length of the received frame
 */
static void mgtProtocolHandler_processFrame(MgtProtocolHandler *me, const uint8_t *frameContent, uint32_t frameLength)
{
    Q_ASSERT(frameContent != (void*)0);
    Q_ASSERT(frameLength >= MGT_PROTOCOL_CMD_NUMBER_SIZE);
    uint32_t commandNumber = ntohl(*((uint32_t*)frameContent));

    /* print command number to display */
    char commandNumberString[COMMAND_NUMBER_STRING_SIZE];
    snprintf(commandNumberString, sizeof(commandNumberString), "0x%x", (unsigned int) commandNumber);
    LCDPutStr(commandNumberString, 80, 40, LARGE, RED, WHITE);

    switch (commandNumber) {
    case MGT_PROTOCOL_CMD_DELIVER_ICE_CUBE:
      mgtProtocolHandler_processDeliverIceCube(me, frameContent + MGT_PROTOCOL_CMD_NUMBER_SIZE, frameLength - MGT_PROTOCOL_CMD_NUMBER_SIZE);
      break;
    default:
      break;  /* unknown command numbers are ignored */
    }
}

/**
 * Process the command DeliverIceCube.
 *
 * @param me the pointer to the current active object
 * @param payload the payload of the received frame
 * @param frameLength the length of the payload
 */
static void mgtProtocolHandler_processDeliverIceCube(MgtProtocolHandler *me, const uint8_t *payload, uint32_t payloadLength)
{
  Uint8Evt *ue;
  ue = Q_NEW(Uint8Evt, DELIVER_ICE_CUBE_SIG);
  ue->data = 5;  /* TODO: use number of ice cubes here */
  QACTIVE_POST(AO_IceMgr, (QEvent *)ue, me);    /* post directly */
}

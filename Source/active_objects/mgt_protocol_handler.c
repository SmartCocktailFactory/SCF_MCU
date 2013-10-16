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
#define MGT_PROTOCOL_VERSION           2u       /**< Program version */
#define MGT_PROTOCOL_CMD_NUMBER_SIZE   4u       /**< Size of command number field in frame */

#define MGT_PROTOCOL_CMD_GET_VERSION              0x01u    /**< Number of GetVersion command */
#define MGT_PROTOCOL_CMD_ANSWER_GET_VERSION       0x02u    /**< Number of AnswerGetVersion command */
#define MGT_PROTOCOL_CMD_DELIVER_ICE_CUBE         0x03u    /**< Number of DeliverIceCube command */
#define MGT_PROTOCOL_CMD_ANSWER_DELIVER_ICE_CUBE  0x04u    /**< Number of AnswerDeliverIceCube command */

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
static void mgtProtocolHandler_generateDeliveryIceCubeAnswer(MgtProtocolHandler *me, uint8_t status);
static void mgtProtocolHandler_processGetVersion(MgtProtocolHandler *me, const uint8_t *payload, uint32_t payloadLength);


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
        case ICE_CUBE_DELIVERY_DONE_SIG: {
            mgtProtocolHandler_generateDeliveryIceCubeAnswer(me, 0u);
            return Q_HANDLED();
        }
        case ICE_CUBE_DELIVERY_TMO_SIG: {
            mgtProtocolHandler_generateDeliveryIceCubeAnswer(me, 2u);
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
    case MGT_PROTOCOL_CMD_GET_VERSION:
      mgtProtocolHandler_processGetVersion(me, frameContent + MGT_PROTOCOL_CMD_NUMBER_SIZE, frameLength - MGT_PROTOCOL_CMD_NUMBER_SIZE);
      break;
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

  if (payloadLength >= 1u) {
      ue = Q_NEW(Uint8Evt, DELIVER_ICE_CUBE_SIG);
      ue->data = payload[0];
      QACTIVE_POST(AO_IceMgr, (QEvent *)ue, me);    /* post directly */
  }
  else {
      mgtProtocolHandler_generateDeliveryIceCubeAnswer(me, 1u);
  }
}

/**
 * Generate an AnswerDeliveryIceCube packet.
 *
 * @param me the pointer to the current active object
 * @param status the delivery status
 */
static void mgtProtocolHandler_generateDeliveryIceCubeAnswer(MgtProtocolHandler *me, uint8_t status) {
  DataEvt *de;
  de = Q_NEW(DataEvt, SEND_UDP_SIG);
  *((uint32_t *)&de->dataBuffer[0]) = htonl(MGT_PROTOCOL_CMD_ANSWER_DELIVER_ICE_CUBE);
  de->dataBuffer[4] = status;
  de->usedDataBufferLength = 5u;
  QACTIVE_POST(AO_LwIPMgr, (QEvent *)de, me);     /* post directly */
}

/**
 * Process the command GetVersion.
 *
 * @param me the pointer to the current active object
 * @param payload the payload of the received frame
 * @param frameLength the length of the payload
 */
static void mgtProtocolHandler_processGetVersion(MgtProtocolHandler *me, const uint8_t *payload, uint32_t payloadLength) {
  (void)payload;
  (void)payloadLength;
  DataEvt *de;
  de = Q_NEW(DataEvt, SEND_UDP_SIG);
  *((uint32_t *)&de->dataBuffer[0]) = htonl(MGT_PROTOCOL_CMD_ANSWER_GET_VERSION);
  *((uint16_t *)&de->dataBuffer[4]) = htons(MGT_PROTOCOL_VERSION);
  de->usedDataBufferLength = 6u;
  QACTIVE_POST(AO_LwIPMgr, (QEvent *)de, me);     /* post directly */
}

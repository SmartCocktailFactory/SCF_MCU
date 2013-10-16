/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        This header file defines all user signals and events and declares
                      all active objects.

 ****************************************************************************************/
#ifndef ao_def_h
#define ao_def_h

#include "qf.h"

enum ScfSignals {
   EAT_SIG = Q_USER_SIG,     /* published by Table to let a philosopher eat */
   DONE_SIG,                   /* published by Philosopher when done eating */
   DISPLAY_IPADDR_SIG,
   DISPLAY_CGI_SIG,
   DISPLAY_UDP_SIG,
   PROCESS_UDP_SIG,        /* DataEvt type; published by lwipmgr if a udp packet has been received */
   ENABLE_RELAY_SIG,       /* Uint8Evt type with relay number; published by iceMgr if relay should be enabled (switched on) */
   DISABLE_RELAY_SIG,      /* Uint8Evt type with relay number; published by iceMgr if relay should be disabled (switched off) */
   ICE_CUBE_DETECTED_SIG,  /* published by ice detection manager, when an ice cube has been detected */
   TERMINATE_SIG,          /* published by BSP to terminate the application */
   MAX_PUB_SIG,                                /* the last published signal */

   DELIVER_ICE_CUBE_SIG,   /* Uint8Evt type; posted by mgtProtocolHandler directly to iceMgr if one or more ice cubes shall be delivered */
   ICE_CUBE_DELIVERY_DONE_SIG,
   ICE_CUBE_DELIVERY_TMO_SIG,
   SEND_UDP_SIG,
   MAX_SIG                                               /* the last signal */
};

#define MAX_TEXT_LEN 16
typedef struct TextEvtTag {
	QEvent super;
	char text[MAX_TEXT_LEN];
} TextEvt;

#define DATA_EVT_BUFFER_SIZE 32
typedef struct DataEvtTag {
  QEvent super;
  uint8_t dataBuffer[DATA_EVT_BUFFER_SIZE];  /* data buffer */
  uint32_t usedDataBufferLength;     /* the number of bytes in the data buffer that are actually used */
} DataEvt;

typedef struct Uint8EvtTag {
  QEvent super;
  uint8_t data;   /* any type of event parameter fitting into one data byte */
} Uint8Evt;

void Table_ctor(void);
void LwIPMgr_ctor(void);
void MgtProtocolHandler_ctor(void);
void IceMgr_ctor(void);
void ModIo2Mgr_ctor(void);
void IceDetectionMgr_ctor(void);

extern QActive * const AO_Table;                /* "opaque" pointer  to Table AO */
extern QActive * const AO_LwIPMgr;              /* "opaque" pointer  to LwIPMgr AO */
extern QActive * const AO_MgtProtocolHandler;   /* "opaque" pointer  to MgtProtocolHandler AO */
extern QActive * const AO_IceMgr;               /* "opaque" pointer  to IceMgr AO */
extern QActive * const AO_ModIo2Mgr;            /* "opaque" pointer  to ModIo2Mgr AO */
extern QActive * const AO_IceDetectionMgr;      /* "opaque" pointer  to IceDetectorMgr AO */

#endif

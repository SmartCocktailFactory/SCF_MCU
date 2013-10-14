/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 07, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
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
#ifndef ao_def_h
#define ao_def_h

#include "qf.h"

enum DPPSignals {
   EAT_SIG = Q_USER_SIG,     /* published by Table to let a philosopher eat */
   DONE_SIG,                   /* published by Philosopher when done eating */
   DISPLAY_IPADDR_SIG,
   DISPLAY_CGI_SIG,
   DISPLAY_UDP_SIG,
   PROCESS_UDP_SIG,        /* DataEvt type; published by lwipmgr if a udp packet has been received */
   ENABLE_RELAY_SIG,       /* Uint8Evt type with relay number; published by iceMgr if relay should be enabled (switched on) */
   DISABLE_RELAY_SIG,      /* Uint8Evt type with relay number; published by iceMgr if relay should be disabled (switched off) */
   TERMINATE_SIG,          /* published by BSP to terminate the application */
   MAX_PUB_SIG,                                /* the last published signal */

   DELIVER_ICE_CUBE_SIG,   /* Uint8Evt type; posted by mgtProtocolHandler directly to iceMgr if one or more ice cubes shall be delivered */
   SEND_UDP_SIG,
   MAX_SIG                                               /* the last signal */
};

#define MAX_TEXT_LEN 16
typedef struct TextEvtTag {
	QEvent super;
	char text[MAX_TEXT_LEN];
} TextEvt;

#define MAX_DATA_LEN 16
typedef struct DataEvtTag {
  QEvent super;
  uint8_t data[MAX_DATA_LEN];
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

extern QActive * const AO_Table;                /* "opaque" pointer  to Table AO */
extern QActive * const AO_LwIPMgr;              /* "opaque" pointer  to LwIPMgr AO */
extern QActive * const AO_MgtProtocolHandler;   /* "opaque" pointer  to MgtProtocolHandler AO */
extern QActive * const AO_IceMgr;               /* "opaque" pointer  to IceMgr AO */
extern QActive * const AO_ModIo2Mgr;            /* "opaque" pointer  to ModIo2Mgr AO */

#endif

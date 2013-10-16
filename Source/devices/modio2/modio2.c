/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Interface of the MODIO2 extension board device.

 ****************************************************************************************/

#include "modio2.h"
#include <omx_p207_eval/i2c/i2c.h>

/** The I2C slave address of MODIO2. */
#define MODIO2_SLAVE_ADDRESS 0x42U

/** Command: Set relay. */
#define MODIO2_COMMAND_SET_RELAY 0x40U

/** The current relay state. */
static uint8_t relayState = 0x00U;

/** Sends the relay state to MODIO2. */
Status Write_RelayState() {
  const uint8_t buf[] = { MODIO2_COMMAND_SET_RELAY, relayState };
  return (omsEval_i2c_write(MODIO2_SLAVE_ADDRESS, sizeof(buf), buf) != ERROR) ? Success : Error;
}

Status ModIO2_enableRelay1() {
  relayState |= 0x01;
  return Write_RelayState();
}

Status ModIO2_disableRelay1() {
  relayState &= (~0x01);
  return Write_RelayState();
}

Status ModIO2_enableRelay2() {
  relayState |= 0x02;
  return Write_RelayState();
}

Status ModIO2_disableRelay2() {
  relayState &= (~0x02);
  return Write_RelayState();
}

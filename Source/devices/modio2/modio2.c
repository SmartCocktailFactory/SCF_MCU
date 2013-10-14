/*
 * modio2.c
 *
 *  Created on: Oct 14, 2013
 *      Author: Daniel Steiner
 */

#include "modio2.h"
#include <omx_p207_eval/i2c/i2c.h>

#define MODIO2_SLAVE_ADDRESS 0x21U
#define MODIO2_COMMAND_SET_RELAY 0x40U

static uint8_t relayState = 0x00U;

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

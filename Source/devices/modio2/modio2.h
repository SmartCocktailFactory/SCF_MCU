/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Interface of the MODIO2 extension board device.

 ****************************************************************************************/

#ifndef MODIO2_H_
#define MODIO2_H_

/** Status of MODIO2 command execution. */
typedef enum {
  Error = 0,        /**< The command failed. */
  Success = !Error  /**< The command succeeded. */
} Status;

/** Initialize the MODIO2 board. */
Status ModIO2_init();

/** Enable the first relay. */
Status ModIO2_enableRelay1();

/** Disable the first relay. */
Status ModIO2_disableRelay1();

/** Enable the second relay. */
Status ModIO2_enableRelay2();

/** Disable the second relay. */
Status ModIO2_disableRelay2();

#endif /* MODIO2_H_ */

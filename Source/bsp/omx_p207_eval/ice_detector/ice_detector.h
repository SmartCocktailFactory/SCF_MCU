/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Header of the ice detection sensor driver.

 ****************************************************************************************/

#ifndef ICE_DETECTOR_H_
#define ICE_DETECTOR_H_

/** The ice detection status. */
typedef enum {
  NO_ICE = 0, /**< No ice detected. */
  ICE         /**< Ice detected. */
} IceStatus;

/** Initialize the ice detection sensor driver. */
void omxEval_ice_init();

/**
 * Get the ice detection status.
 *
 * @return \c ICE or \c NO_ICE depending whether the sensor detects ice.
 */
IceStatus omsEval_ice_status();

#endif /* ICE_DETECTOR_H_ */

/****************************************************************************************
                         Copyright (C) Adrian Lutz
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Utility functions for Olimex P207 eval board.

 ****************************************************************************************

 ****************************************************************************************
 Includes
*****************************************************************************************/
#include "utils.h"
#include <stdint.h>

/****************************************************************************************
 Defines
*****************************************************************************************/
#define DELAY_CYCLES_PER_MICROSECOND  6u


/****************************************************************************************
 Enums
*****************************************************************************************/


/****************************************************************************************
 Structs and typedefs
*****************************************************************************************/


/****************************************************************************************
 Macros
*****************************************************************************************/


/****************************************************************************************
 Forward declarations
*****************************************************************************************/


/****************************************************************************************
 Local data
*****************************************************************************************/


/****************************************************************************************
 Public function implementations
*****************************************************************************************/
__attribute__((optimize(0)))   /* disable optimization for this function to make timing independent of optimization level */
void delayUs(uint32_t delayInMicroseconds)
{
  volatile uint32_t i = 0;

  /* number of delay cycles has been verified on 15.08.2013 using the oscilloscope (for -O0 and -Os optimization settings) */
  for(i = 0; i < delayInMicroseconds*DELAY_CYCLES_PER_MICROSECOND; i++) {
    /* do nothing */
  }
}

__attribute__((optimize(0)))   /* disable optimization for this function to make timing independent of optimization level */
void delayMs(uint32_t delayInMilliseconds)
{
  volatile uint32_t i = 0;

  /* delay has been verified on 15.08.2013 using the oscilloscope (for -O0 and -Os optimization settings) */
  for(i = 0; i < delayInMilliseconds; i++) {
    delayUs(1000);
  }
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/
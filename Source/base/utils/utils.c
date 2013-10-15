/****************************************************************************************
                         Copyright (C) Adrian Lutz
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Utility functions Utility functions for various tasks.

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

#define LITTLE_ENDIAN
uint32_t htonl(uint32_t hostlong)
{
#if defined(LITTLE_ENDIAN)
  return ((hostlong & 0xff000000ul) >> 24) |
         ((hostlong & 0x00ff0000ul) >> 8) |
         ((hostlong & 0x0000ff00ul) << 8) |
         ((hostlong & 0x000000fful) << 24);
#elif defined(BIG_ENDIAN)
  return hostlong;
#else
  error "Endianness not defined"
#endif
}

uint16_t htons(uint16_t hostshort)
{
#if defined(LITTLE_ENDIAN)
  return ((hostshort & 0xff00u) >> 8) |
         ((hostshort & 0x00ffu) << 8);
#elif defined(BIG_ENDIAN)
  return hostshort;
#else
  error "Endianness not defined"
#endif
}

uint32_t ntohl(uint32_t netlong)
{
  return htonl(netlong);
}

uint16_t ntohs(uint16_t netshort)
{
  return htons(netshort);
}


/****************************************************************************************
 Private function implementations
*****************************************************************************************/

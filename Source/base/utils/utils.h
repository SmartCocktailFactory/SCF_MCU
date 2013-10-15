/****************************************************************************************
                         Copyright (C) Adrian Lutz
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Utility functions for various tasks.


 ****************************************************************************************
 ****************************************************************************************/
#ifndef UTILS_H_
#define UTILS_H_

/****************************************************************************************
 Includes
*****************************************************************************************/
#include <stdint.h>


/****************************************************************************************
 Defines
*****************************************************************************************/


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
 Public function declarations
*****************************************************************************************/
/**
 * Generate a delay in microseconds.
 * The function busy waits for the specified delay and then returns.
 *
 * @param delayInMicroseconds the delay in microseconds
 */
void delayUs(uint32_t delayInMicroseconds);

/**
 * Generate a delay in milliseconds.
 * The function busy waits for the specified delay and then returns.
 *
 * @param delayInMilliseconds the delay in milliseconds
 */
void delayMs(uint32_t delayInMilliseconds);

/**
 * Convert a 32 bit value from host byte order to network byte order.
 *
 * @param hostlong the value to be converted
 * @return the converted value
 */
uint32_t htonl(uint32_t hostlong);

/**
 * Convert a 16 bit value from host byte order to network byte order.
 *
 * @param hostshort the value to be converted
 * @return the converted value
 */
uint16_t htons(uint16_t hostshort);

/**
 * Convert a 32 bit value from network byte order to host byte order.
 *
 * @param netlong the value to be converted
 * @return the converted value
 */
uint32_t ntohl(uint32_t netlong);

/**
 * Convert a 16 bit value from network byte order to host byte order.
 *
 * @param netshort the value to be converted
 * @return the converted value
 */
uint16_t ntohs(uint16_t netshort);

#endif

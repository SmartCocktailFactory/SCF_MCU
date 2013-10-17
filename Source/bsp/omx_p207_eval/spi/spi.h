/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Header of the SPI driver.

 ****************************************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include <STM32F2xx_StdPeriph_Driver/inc/stm32f2xx_spi.h>
#include <stddef.h>

/**
 * Initialize the SPI port on the Olimex eval board.
 */
void omxEval_spi_init();

/**
 * Write data to an SPI slave device.
 *
 * @param address The address of the SPI salve device.
 * @param length The number of bytes to write.
 * @param data The data buffer to send. Must contain at least \c length bytes.
 */
ErrorStatus omsEval_spi_write(uint8_t address, size_t length, const uint8_t* data);

/**
 * Read/write data from/to an SPI slave device.
 *
 * @param address The address of the SPI salve device.
 * @param length The number of bytes to read.
 * @param out The data buffer to send. Must contain at least \c length bytes.
 * @param in The buffer to read the data to. Must contain at least \c length bytes.
 */
ErrorStatus omsEval_spi_readWrite(uint8_t address, size_t length, const uint8_t* out, uint8_t* in);


#endif /* SPI_H_ */

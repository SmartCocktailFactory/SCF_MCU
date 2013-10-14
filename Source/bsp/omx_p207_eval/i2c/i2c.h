/*
 * i2c.h
 *
 *  Created on: Oct 04, 2013
 *      Author: Daniel Steiner
 */

#ifndef I2C_H_
#define I2C_H_

#include <STM32F2xx_StdPeriph_Driver/inc/stm32f2xx_i2c.h>
#include <stddef.h>

/**
 * Initialize the I2C ports on the Olimex eval board.
 */
void omxEval_i2c_init();

/**
 * Write data to an i2c slave device.
 *
 * @param address The address of the i2c salve device.
 * @param length The number of bytes to write.
 * @param data The data buffer to send. Must contain at least \c length bytes.
 */
ErrorStatus omsEval_i2c_write(uint8_t address, size_t length, const uint8_t* data);

/**
 * Read data from an i2c slave device.
 *
 * @param address The address of the i2c salve device.
 * @param length The number of bytes to read.
 * @param data The buffer to read the data to. Must contain at least \c length bytes.
 */
ErrorStatus omsEval_i2c_read(uint8_t address, size_t length, uint8_t* data);

#endif /* I2C_H_ */

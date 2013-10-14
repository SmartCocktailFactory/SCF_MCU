/*
 * i2c.c
 *
 *  Created on: Oct 04, 2013
 *      Author: Daniel Steiner
 */

#include "i2c.h"

#define OMX_EVAL_I2C_MODIO2                 I2C2 // I2C2 : PF0 and PF1
#define OMX_EVAL_I2C_MODIO2_CLK             RCC_APB1Periph_I2C2
#define OMX_EVAL_I2C_MODIO2_SDA_PORT        GPIOF // PF0
#define OMX_EVAL_I2C_MODIO2_SDA_CLK         RCC_AHB1Periph_GPIOF
#define OMX_EVAL_I2C_MODIO2_SDA_SOURCE      GPIO_PinSource0
#define OMX_EVAL_I2C_MODIO2_SDA_AF          GPIO_AF_I2C2
#define OMX_EVAL_I2C_MODIO2_SDA_PIN         GPIO_Pin_0
#define OMX_EVAL_I2C_MODIO2_SCL_PORT        GPIOF // PF1
#define OMX_EVAL_I2C_MODIO2_SCL_CLK         RCC_AHB1Periph_GPIOF
#define OMX_EVAL_I2C_MODIO2_SCL_SOURCE      GPIO_PinSource1
#define OMX_EVAL_I2C_MODIO2_SCL_AF          GPIO_AF_I2C2
#define OMX_EVAL_I2C_MODIO2_SCL_PIN         GPIO_Pin_1

void omxEval_i2c_init() {

  I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(
  OMX_EVAL_I2C_MODIO2_SDA_CLK | OMX_EVAL_I2C_MODIO2_SCL_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(OMX_EVAL_I2C_MODIO2_CLK, ENABLE);

  RCC_APB1PeriphResetCmd(OMX_EVAL_I2C_MODIO2_CLK, ENABLE);
  RCC_APB1PeriphResetCmd(OMX_EVAL_I2C_MODIO2_CLK, DISABLE);

  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_I2C_MODIO2_SDA_PIN;
  GPIO_Init(OMX_EVAL_I2C_MODIO2_SDA_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_I2C_MODIO2_SCL_PIN;
  GPIO_Init(OMX_EVAL_I2C_MODIO2_SCL_PORT, &GPIO_InitStructure);

  GPIO_PinAFConfig(OMX_EVAL_I2C_MODIO2_SDA_PORT, OMX_EVAL_I2C_MODIO2_SDA_SOURCE,
  OMX_EVAL_I2C_MODIO2_SDA_AF);
  GPIO_PinAFConfig(OMX_EVAL_I2C_MODIO2_SCL_PORT, OMX_EVAL_I2C_MODIO2_SCL_SOURCE,
  OMX_EVAL_I2C_MODIO2_SCL_AF);

  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x11;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(OMX_EVAL_I2C_MODIO2, &I2C_InitStructure);

  I2C_Cmd(OMX_EVAL_I2C_MODIO2, ENABLE);
}

ErrorStatus omsEval_i2c_write(uint8_t address, size_t length, const uint8_t* data) {

  __IO uint32_t Timeout = 0U;

  /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
  //OMX_EVAL_I2C_MODIO2->CR2 |= I2C_IT_ERR;
  I2C_GenerateSTART(OMX_EVAL_I2C_MODIO2, ENABLE);
  Timeout = 0xFFFFU;
  while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_MODE_SELECT)) {
    if (Timeout-- == 0U)
      return ERROR;
  }

  I2C_Send7bitAddress(OMX_EVAL_I2C_MODIO2, address, I2C_Direction_Transmitter);
  Timeout = 0xFFFFU;
  while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    if (Timeout-- == 0U)
      return ERROR;
  }

  while (length > 0U) {
    Timeout = 0xFFFFU;
    I2C_SendData(OMX_EVAL_I2C_MODIO2, *data);
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
      if (Timeout-- == 0U)
        return ERROR;
    }
    data++;
    length--;
  }

  I2C_GenerateSTOP(OMX_EVAL_I2C_MODIO2, ENABLE);
  Timeout = 0xFFFFU;
  while ((I2C_ReadRegister(OMX_EVAL_I2C_MODIO2, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
    if (Timeout-- == 0U)
      return ERROR;
  }

  return SUCCESS;
}

ErrorStatus omsEval_i2c_read(uint8_t address, size_t length, uint8_t* data) {

  __IO uint32_t Timeout = 0;

  /* Enable I2C errors interrupts (used in all modes: Polling, DMA and Interrupts */
  //OMX_EVAL_I2C_MODIO2->CR2 |= I2C_IT_ERR;
  if (length == 1U) {

    I2C_GenerateSTART(OMX_EVAL_I2C_MODIO2, ENABLE);
    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_MODE_SELECT)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    I2C_Send7bitAddress(OMX_EVAL_I2C_MODIO2, address, I2C_Direction_Receiver);
    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    I2C_AcknowledgeConfig(OMX_EVAL_I2C_MODIO2, DISABLE);
    __disable_irq();
    (void) I2C_ReadRegister(OMX_EVAL_I2C_MODIO2, I2C_Register_SR2);
    I2C_GenerateSTOP(OMX_EVAL_I2C_MODIO2, ENABLE);
    __enable_irq();

    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);

    Timeout = 0xFFFFU;
    while ((I2C_ReadRegister(OMX_EVAL_I2C_MODIO2, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
      if (Timeout-- == 0U)
        return ERROR;
    }
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_MODIO2, ENABLE);

  } else if (length == 2U) {

    I2C_NACKPositionConfig(OMX_EVAL_I2C_MODIO2, I2C_NACKPosition_Next);
    I2C_GenerateSTART(OMX_EVAL_I2C_MODIO2, ENABLE);
    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_MODE_SELECT)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    I2C_Send7bitAddress(OMX_EVAL_I2C_MODIO2, address, I2C_Direction_Receiver);
    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    __disable_irq();
    (void) I2C_ReadRegister(OMX_EVAL_I2C_MODIO2, I2C_Register_SR2);
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_MODIO2, DISABLE);
    __enable_irq();

    Timeout = 0xFFFFU;
    while (!I2C_GetFlagStatus(OMX_EVAL_I2C_MODIO2, I2C_FLAG_BTF)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    __disable_irq();
    I2C_GenerateSTOP(OMX_EVAL_I2C_MODIO2, ENABLE);
    *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);
    data++;
    __enable_irq();

    *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);

    Timeout = 0xFFFFU;
    while ((I2C_ReadRegister(OMX_EVAL_I2C_MODIO2, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
      if (Timeout-- == 0U)
        return ERROR;
    }
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_MODIO2, ENABLE);
    I2C_NACKPositionConfig(OMX_EVAL_I2C_MODIO2, I2C_NACKPosition_Current);

  } else {

    I2C_GenerateSTART(OMX_EVAL_I2C_MODIO2, ENABLE);
    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_MODE_SELECT)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    I2C_Send7bitAddress(OMX_EVAL_I2C_MODIO2, address, I2C_Direction_Receiver);
    Timeout = 0xFFFFU;
    while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
      if (Timeout-- == 0U)
        return ERROR;
    }

    while (length > 0U) {
      Timeout = 0xFFFFU;
      while (!I2C_GetFlagStatus(OMX_EVAL_I2C_MODIO2, I2C_FLAG_BTF)) {
        if (Timeout-- == 0U)
          return ERROR;
      }
      if (length > 3U) {
        *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);
        data++;
        length--;
      } else {

        I2C_AcknowledgeConfig(OMX_EVAL_I2C_MODIO2, DISABLE);

        __disable_irq();
        *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);
        data++;
        I2C_GenerateSTOP(OMX_EVAL_I2C_MODIO2, ENABLE);
        *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);
        data++;
        __enable_irq();

        Timeout = 0xFFFFU;
        while (!I2C_CheckEvent(OMX_EVAL_I2C_MODIO2, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
          if (Timeout-- == 0U)
            return ERROR;
        }

        *data = I2C_ReceiveData(OMX_EVAL_I2C_MODIO2);
        length = 0U;

      }
    }
    Timeout = 0xFFFFU;
    while ((I2C_ReadRegister(OMX_EVAL_I2C_MODIO2, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
      if (Timeout-- == 0U)
        return ERROR;
    }
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_MODIO2, ENABLE);
  }

  return SUCCESS;
}

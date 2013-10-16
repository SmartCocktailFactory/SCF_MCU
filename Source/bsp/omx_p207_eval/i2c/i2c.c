/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Implementation of the I2C driver.

 ****************************************************************************************/

#include "i2c.h"

/** I2C port configuration. */
#define OMX_EVAL_I2C_UEXT                 I2C2 // I2C2 : PF0 and PF1
#define OMX_EVAL_I2C_UEXT_CLK             RCC_APB1Periph_I2C2
#define OMX_EVAL_I2C_UEXT_EV_IRQn         I2C2_EV_IRQn
#define OMX_EVAL_I2C_UEXT_ER_IRQn         I2C2_ER_IRQn
#define OMX_EVAL_I2C_UEXT_EV_IRQHANDLER   I2C2_EV_IRQHandler
#define OMX_EVAL_I2C_UEXT_ER_IRQHANDLER   I2C2_ER_IRQHandler
#define OMX_EVAL_I2C_UEXT_SDA_PORT        GPIOF // PF0
#define OMX_EVAL_I2C_UEXT_SDA_CLK         RCC_AHB1Periph_GPIOF
#define OMX_EVAL_I2C_UEXT_SDA_SOURCE      GPIO_PinSource0
#define OMX_EVAL_I2C_UEXT_SDA_AF          GPIO_AF_I2C2
#define OMX_EVAL_I2C_UEXT_SDA_PIN         GPIO_Pin_0
#define OMX_EVAL_I2C_UEXT_SCL_PORT        GPIOF // PF1
#define OMX_EVAL_I2C_UEXT_SCL_CLK         RCC_AHB1Periph_GPIOF
#define OMX_EVAL_I2C_UEXT_SCL_SOURCE      GPIO_PinSource1
#define OMX_EVAL_I2C_UEXT_SCL_AF          GPIO_AF_I2C2
#define OMX_EVAL_I2C_UEXT_SCL_PIN         GPIO_Pin_1

/** Configure and enable the I2C interface. */
static void Enable_Interface() {

  I2C_InitTypeDef I2C_InitStructure;

  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x11;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  I2C_DeInit(OMX_EVAL_I2C_UEXT);
  I2C_Init(OMX_EVAL_I2C_UEXT, &I2C_InitStructure);

  I2C_ITConfig(OMX_EVAL_I2C_UEXT, I2C_IT_ERR, ENABLE);
  I2C_Cmd(OMX_EVAL_I2C_UEXT, ENABLE);
}

/** Disable the I2C interface. */
static void Disable_Interface() {

  I2C_ITConfig(OMX_EVAL_I2C_UEXT, I2C_IT_ERR, DISABLE);
  I2C_Cmd(OMX_EVAL_I2C_UEXT, DISABLE);
}

/** Interrupt handler for I2C error interrupt. */
void OMX_EVAL_I2C_UEXT_ER_IRQHANDLER(void) {
  if ((I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_SR1) & 0xFF00) != 0x00) {
    OMX_EVAL_I2C_UEXT->SR1 &= 0x00FF; // clear error flags
  }
}

/** Interrupt handler for I2C event interrupt. */
void OMX_EVAL_I2C_UEXT_EV_IRQHANDLER(void) {
}

void omxEval_i2c_init() {

  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(OMX_EVAL_I2C_UEXT_SDA_CLK | OMX_EVAL_I2C_UEXT_SCL_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(OMX_EVAL_I2C_UEXT_CLK, ENABLE);

  RCC_APB1PeriphResetCmd(OMX_EVAL_I2C_UEXT_CLK, ENABLE);
  RCC_APB1PeriphResetCmd(OMX_EVAL_I2C_UEXT_CLK, DISABLE);

  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_I2C_UEXT_SDA_PIN;
  GPIO_Init(OMX_EVAL_I2C_UEXT_SDA_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_I2C_UEXT_SCL_PIN;
  GPIO_Init(OMX_EVAL_I2C_UEXT_SCL_PORT, &GPIO_InitStructure);

  GPIO_PinAFConfig(OMX_EVAL_I2C_UEXT_SDA_PORT, OMX_EVAL_I2C_UEXT_SDA_SOURCE, OMX_EVAL_I2C_UEXT_SDA_AF);
  GPIO_PinAFConfig(OMX_EVAL_I2C_UEXT_SCL_PORT, OMX_EVAL_I2C_UEXT_SCL_SOURCE, OMX_EVAL_I2C_UEXT_SCL_AF);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_InitStructure.NVIC_IRQChannel = OMX_EVAL_I2C_UEXT_EV_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = OMX_EVAL_I2C_UEXT_ER_IRQn;
  NVIC_Init(&NVIC_InitStructure);
}

ErrorStatus omsEval_i2c_write(uint8_t address, size_t length, const uint8_t* data) {

  __IO uint32_t timeout = 0U;

  Enable_Interface();

  I2C_GenerateSTART(OMX_EVAL_I2C_UEXT, ENABLE);
  timeout = 0xFFFFU;
  while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) {
    if (timeout-- == 0U) {
      Disable_Interface();
      return ERROR;
    }
  }

  I2C_Send7bitAddress(OMX_EVAL_I2C_UEXT, address, I2C_Direction_Transmitter);
  timeout = 0xFFFFU;
  while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) {
    if (timeout-- == 0U) {
      Disable_Interface();
      return ERROR;
    }
  }

  while (length > 0U) {
    timeout = 0xFFFFU;
    I2C_SendData(OMX_EVAL_I2C_UEXT, *data);
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    data++;
    length--;
  }

  I2C_GenerateSTOP(OMX_EVAL_I2C_UEXT, ENABLE);
  timeout = 0xFFFFU;
  while ((I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
    if (timeout-- == 0U) {
      Disable_Interface();
      return ERROR;
    }
  }

  Disable_Interface();
  return SUCCESS;
}

ErrorStatus omsEval_i2c_read(uint8_t address, size_t length, uint8_t* data) {

  __IO uint32_t timeout = 0U;

  Enable_Interface();

  if (length == 1U) {

    I2C_GenerateSTART(OMX_EVAL_I2C_UEXT, ENABLE);
    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    I2C_Send7bitAddress(OMX_EVAL_I2C_UEXT, address, I2C_Direction_Receiver);
    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    I2C_AcknowledgeConfig(OMX_EVAL_I2C_UEXT, DISABLE);
    __disable_irq();
    (void) I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_SR2);
    I2C_GenerateSTOP(OMX_EVAL_I2C_UEXT, ENABLE);
    __enable_irq();

    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);

    timeout = 0xFFFFU;
    while ((I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_UEXT, ENABLE);

  } else if (length == 2U) {

    I2C_NACKPositionConfig(OMX_EVAL_I2C_UEXT, I2C_NACKPosition_Next);
    I2C_GenerateSTART(OMX_EVAL_I2C_UEXT, ENABLE);
    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    I2C_Send7bitAddress(OMX_EVAL_I2C_UEXT, address, I2C_Direction_Receiver);
    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    __disable_irq();
    (void) I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_SR2);
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_UEXT, DISABLE);
    __enable_irq();

    timeout = 0xFFFFU;
    while (I2C_GetFlagStatus(OMX_EVAL_I2C_UEXT, I2C_FLAG_BTF) == RESET) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    __disable_irq();
    I2C_GenerateSTOP(OMX_EVAL_I2C_UEXT, ENABLE);
    *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);
    data++;
    __enable_irq();

    *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);

    timeout = 0xFFFFU;
    while ((I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_UEXT, ENABLE);
    I2C_NACKPositionConfig(OMX_EVAL_I2C_UEXT, I2C_NACKPosition_Current);

  } else {

    I2C_GenerateSTART(OMX_EVAL_I2C_UEXT, ENABLE);
    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    I2C_Send7bitAddress(OMX_EVAL_I2C_UEXT, address, I2C_Direction_Receiver);
    timeout = 0xFFFFU;
    while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }

    while (length > 0U) {
      timeout = 0xFFFFU;
      while (I2C_GetFlagStatus(OMX_EVAL_I2C_UEXT, I2C_FLAG_BTF) == RESET) {
        if (timeout-- == 0U) {
          Disable_Interface();
          return ERROR;
        }
      }
      if (length > 3U) {
        *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);
        data++;
        length--;
      } else {

        I2C_AcknowledgeConfig(OMX_EVAL_I2C_UEXT, DISABLE);

        __disable_irq();
        *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);
        data++;
        I2C_GenerateSTOP(OMX_EVAL_I2C_UEXT, ENABLE);
        *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);
        data++;
        __enable_irq();

        timeout = 0xFFFFU;
        while (I2C_CheckEvent(OMX_EVAL_I2C_UEXT, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) {
          if (timeout-- == 0U) {
            Disable_Interface();
            return ERROR;
          }
        }

        *data = I2C_ReceiveData(OMX_EVAL_I2C_UEXT);
        length = 0U;

      }
    }
    timeout = 0xFFFFU;
    while ((I2C_ReadRegister(OMX_EVAL_I2C_UEXT, I2C_Register_CR1) & I2C_CR1_STOP) == I2C_CR1_STOP) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    I2C_AcknowledgeConfig(OMX_EVAL_I2C_UEXT, ENABLE);
  }

  return SUCCESS;
}

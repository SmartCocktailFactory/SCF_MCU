/****************************************************************************************
 Copyright (C) Zuehlke Engineering
 All Rights Reserved
 ****************************************************************************************

 DESCRIPTION:        Implementation of the SPI driver.

 ****************************************************************************************/

#include "spi.h"

/** SPI port configuration. */
#define OMX_EVAL_SPI_UEXT                 SPI3 // SPI3 : PF0 and PF1
#define OMX_EVAL_SPI_UEXT_CLK             RCC_APB1Periph_SPI3

#define OMX_EVAL_SPI_UEXT_SCK_PORT        GPIOC // PC10
#define OMX_EVAL_SPI_UEXT_SCK_CLK         RCC_AHB1Periph_GPIOC
#define OMX_EVAL_SPI_UEXT_SCK_PIN         GPIO_Pin_10
#define OMX_EVAL_SPI_UEXT_SCK_SOURCE      GPIO_PinSource10
#define OMX_EVAL_SPI_UEXT_SCK_AF          GPIO_AF_SPI3

#define OMX_EVAL_SPI_UEXT_MISO_PORT       GPIOC // PC11
#define OMX_EVAL_SPI_UEXT_MISO_CLK        RCC_AHB1Periph_GPIOC
#define OMX_EVAL_SPI_UEXT_MISO_PIN        GPIO_Pin_11
#define OMX_EVAL_SPI_UEXT_MISO_SOURCE     GPIO_PinSource11
#define OMX_EVAL_SPI_UEXT_MISO_AF         GPIO_AF_SPI3

#define OMX_EVAL_SPI_UEXT_MOSI_PORT       GPIOC // PC12
#define OMX_EVAL_SPI_UEXT_MOSI_CLK        RCC_AHB1Periph_GPIOC
#define OMX_EVAL_SPI_UEXT_MOSI_PIN        GPIO_Pin_12
#define OMX_EVAL_SPI_UEXT_MOSI_SOURCE     GPIO_PinSource12
#define OMX_EVAL_SPI_UEXT_MOSI_AF         GPIO_AF_SPI3

#define OMX_EVAL_SPI_UEXT_CS_PORT         GPIOF // PF8
#define OMX_EVAL_SPI_UEXT_CS_CLK          RCC_AHB1Periph_GPIOF
#define OMX_EVAL_SPI_UEXT_CS_PIN          GPIO_Pin_8

/** Configure and enable the I2C interface. */
static void Enable_Interface() {

  SPI_InitTypeDef SPI_InitStructure;

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_I2S_DeInit(OMX_EVAL_SPI_UEXT);
  SPI_Init(OMX_EVAL_SPI_UEXT, &SPI_InitStructure);

  SPI_Cmd(OMX_EVAL_SPI_UEXT, ENABLE);
  GPIO_ResetBits(OMX_EVAL_SPI_UEXT_CS_PORT, OMX_EVAL_SPI_UEXT_CS_PIN);
}

/** Disable the I2C interface. */
static void Disable_Interface() {

  GPIO_SetBits(OMX_EVAL_SPI_UEXT_CS_PORT, OMX_EVAL_SPI_UEXT_CS_PIN);
  SPI_Cmd(OMX_EVAL_SPI_UEXT, DISABLE);
}

void omxEval_spi_init() {

  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(OMX_EVAL_SPI_UEXT_CLK, ENABLE);

  RCC_AHB1PeriphClockCmd(OMX_EVAL_SPI_UEXT_SCK_CLK | OMX_EVAL_SPI_UEXT_MISO_CLK | OMX_EVAL_SPI_UEXT_MOSI_CLK |
      OMX_EVAL_SPI_UEXT_CS_CLK, ENABLE);

  GPIO_PinAFConfig(OMX_EVAL_SPI_UEXT_SCK_PORT, OMX_EVAL_SPI_UEXT_SCK_SOURCE, OMX_EVAL_SPI_UEXT_SCK_AF);
  GPIO_PinAFConfig(OMX_EVAL_SPI_UEXT_MOSI_PORT, OMX_EVAL_SPI_UEXT_MOSI_SOURCE, OMX_EVAL_SPI_UEXT_MOSI_AF);
  GPIO_PinAFConfig(OMX_EVAL_SPI_UEXT_MISO_PORT, OMX_EVAL_SPI_UEXT_MISO_SOURCE, OMX_EVAL_SPI_UEXT_MISO_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_SPI_UEXT_SCK_PIN;
  GPIO_Init(OMX_EVAL_SPI_UEXT_SCK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_SPI_UEXT_MOSI_PIN;
  GPIO_Init(OMX_EVAL_SPI_UEXT_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_SPI_UEXT_MISO_PIN;
  GPIO_Init(OMX_EVAL_SPI_UEXT_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_SPI_UEXT_CS_PIN;
  GPIO_Init(OMX_EVAL_SPI_UEXT_CS_PORT, &GPIO_InitStructure);

  GPIO_SetBits(OMX_EVAL_SPI_UEXT_CS_PORT, OMX_EVAL_SPI_UEXT_CS_PIN);
}

ErrorStatus omsEval_spi_write(size_t length, const uint8_t* data) {

  __IO uint32_t timeout = 0U;

  Enable_Interface();

  while (length > 0U) {
    timeout = 0xFFFFU;
    while (SPI_I2S_GetFlagStatus(OMX_EVAL_SPI_UEXT, SPI_I2S_FLAG_TXE) == RESET) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    SPI_I2S_SendData(OMX_EVAL_SPI_UEXT, *data);
    length--;
    data++;
  }

  Disable_Interface();
  return SUCCESS;
}

ErrorStatus omsEval_spi_readWrite(size_t length, const uint8_t* out, uint8_t* in) {

  __IO uint32_t timeout = 0U;

  Enable_Interface();

  while (length > 0U) {
    timeout = 0xFFFFU;
    while (SPI_I2S_GetFlagStatus(OMX_EVAL_SPI_UEXT, SPI_I2S_FLAG_TXE) == RESET) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    SPI_I2S_SendData(OMX_EVAL_SPI_UEXT, *out);
    timeout = 0xFFFFU;
    while (SPI_I2S_GetFlagStatus(OMX_EVAL_SPI_UEXT, SPI_I2S_FLAG_RXNE) == RESET) {
      if (timeout-- == 0U) {
        Disable_Interface();
        return ERROR;
      }
    }
    *in = (uint8_t) SPI_I2S_ReceiveData(OMX_EVAL_SPI_UEXT);
    length--;
    in++;
    out++;
  }

  Disable_Interface();
  return SUCCESS;

}

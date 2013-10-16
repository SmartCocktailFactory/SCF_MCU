/****************************************************************************************
                         Copyright (C) Zuehlke Engineering
                              All Rights Reserved
 ****************************************************************************************

  DESCRIPTION:        Implementation of the ice detection sensor driver.

 ****************************************************************************************/

#include "ice_detector.h"
#include <STM32F2xx_StdPeriph_Driver/inc/stm32f2xx_gpio.h>

#define OMX_EVAL_ICE_PORT        GPIOF // PF2
#define OMX_EVAL_ICE_CLK         RCC_AHB1Periph_GPIOF
#define OMX_EVAL_ICE_PIN         GPIO_Pin_2


void omxEval_ice_init() {

  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(OMX_EVAL_ICE_CLK, ENABLE);

  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = OMX_EVAL_ICE_PIN;
  GPIO_Init(OMX_EVAL_ICE_PORT, &GPIO_InitStructure);
}

IceStatus omsEval_ice_status() {
  return (GPIO_ReadInputDataBit(OMX_EVAL_ICE_PORT, OMX_EVAL_ICE_PIN) == 1U) ? ICE : NO_ICE;
}

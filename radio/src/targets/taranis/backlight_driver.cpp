/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x 
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"

#if defined(PCBX9E)
void backlightInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = BACKLIGHT_GPIO_PIN_1|BACKLIGHT_GPIO_PIN_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(BACKLIGHT_GPIO, &GPIO_InitStructure);
  GPIO_PinAFConfig(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PinSource_1, BACKLIGHT_GPIO_AF_1);
  GPIO_PinAFConfig(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PinSource_2, BACKLIGHT_GPIO_AF_1);
  BACKLIGHT_TIMER->ARR = 100 ;
  BACKLIGHT_TIMER->PSC = (PERI2_FREQUENCY * TIMER_MULT_APB2) / 50000 - 1;  // 20us * 100 = 2ms => 500Hz
  BACKLIGHT_TIMER->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 ; // PWM
  BACKLIGHT_TIMER->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E ;
  BACKLIGHT_TIMER->CCR2 = 0 ;
  BACKLIGHT_TIMER->CCR1 = 100 ;
  BACKLIGHT_TIMER->EGR = 0 ;
  BACKLIGHT_TIMER->CR1 = TIM_CR1_CEN ;            // Counter enable
}

void turnBacklightOn(uint8_t level, uint8_t color)
{
  BACKLIGHT_TIMER->CCR1 = ((100-level)*(20-color))/20;
  BACKLIGHT_TIMER->CCR2 = ((100-level)*color)/20;
}

void turnBacklightOff(void)
{
  BACKLIGHT_TIMER->CCR1 = 0;
  BACKLIGHT_TIMER->CCR2 = 0;
}
#elif defined(PCBX9DP)
void backlightInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = BACKLIGHT_GPIO_PIN_1|BACKLIGHT_GPIO_PIN_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(BACKLIGHT_GPIO, &GPIO_InitStructure);
  GPIO_PinAFConfig(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PinSource_1, BACKLIGHT_GPIO_AF_1);
  GPIO_PinAFConfig(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PinSource_2, BACKLIGHT_GPIO_AF_1);
  BACKLIGHT_TIMER->ARR = 100 ;
  BACKLIGHT_TIMER->PSC = (PERI1_FREQUENCY * TIMER_MULT_APB1) / 50000 - 1;  // 20us * 100 = 2ms => 500Hz
  BACKLIGHT_TIMER->CCMR1 = TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 ; // PWM
  BACKLIGHT_TIMER->CCMR2 = TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 ; // PWM
  BACKLIGHT_TIMER->CCER = TIM_CCER_CC4E | TIM_CCER_CC2E ;
  BACKLIGHT_TIMER->CCR2 = 0 ;
  BACKLIGHT_TIMER->CCR4 = 100 ;
  BACKLIGHT_TIMER->EGR = 0 ;
  BACKLIGHT_TIMER->CR1 = TIM_CR1_CEN ;            // Counter enable
}

void turnBacklightOn(uint8_t level, uint8_t color)
{
  BACKLIGHT_TIMER->CCR4 = ((100-level)*(20-color))/20;
  BACKLIGHT_TIMER->CCR2 = ((100-level)*color)/20;
}

void turnBacklightOff(void)
{
  BACKLIGHT_TIMER->CCR4 = 0;
  BACKLIGHT_TIMER->CCR2 = 0;
}
#else
void backlightInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = BACKLIGHT_GPIO_PIN_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(BACKLIGHT_GPIO, &GPIO_InitStructure);
  GPIO_PinAFConfig(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PinSource_1, BACKLIGHT_GPIO_AF_1);
  BACKLIGHT_TIMER->ARR = 100 ;
  BACKLIGHT_TIMER->PSC = (PERI2_FREQUENCY * TIMER_MULT_APB2) / 50000 - 1;  // 20us * 100 = 2ms => 500Hz
  BACKLIGHT_TIMER->CCMR1 = 0x60 ;    // PWM
  BACKLIGHT_TIMER->CCER = 1 ;
  BACKLIGHT_TIMER->CCR1 = 80;
  BACKLIGHT_TIMER->EGR = 0 ;
  BACKLIGHT_TIMER->CR1 = 1 ;
}
#endif
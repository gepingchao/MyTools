/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2020 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADDR_A_Pin GPIO_PIN_13
#define ADDR_A_GPIO_Port GPIOC
#define ADDR_B_Pin GPIO_PIN_14
#define ADDR_B_GPIO_Port GPIOC
#define RS485_RD_Pin GPIO_PIN_1
#define RS485_RD_GPIO_Port GPIOA
#define LCD_A0_Pin GPIO_PIN_4
#define LCD_A0_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_5
#define LCD_RST_GPIO_Port GPIOC
#define LCD_LED_CTRL_Pin GPIO_PIN_0
#define LCD_LED_CTRL_GPIO_Port GPIOB
#define RELAY_CTRL_Pin GPIO_PIN_1
#define RELAY_CTRL_GPIO_Port GPIOB
#define BOOST_EN_Pin GPIO_PIN_12
#define BOOST_EN_GPIO_Port GPIOB
#define SIGNAL_A_Pin GPIO_PIN_13
#define SIGNAL_A_GPIO_Port GPIOB
#define SIGNAL_B_Pin GPIO_PIN_14
#define SIGNAL_B_GPIO_Port GPIOB
#define SIGNAL_D_Pin GPIO_PIN_15
#define SIGNAL_D_GPIO_Port GPIOB
#define USB_D_UP_Pin GPIO_PIN_6
#define USB_D_UP_GPIO_Port GPIOC
#define KEY_1_Pin GPIO_PIN_7
#define KEY_1_GPIO_Port GPIOC
#define KEY_2_Pin GPIO_PIN_8
#define KEY_2_GPIO_Port GPIOC
#define KEY_3_Pin GPIO_PIN_9
#define KEY_3_GPIO_Port GPIOC
#define KEY_4_Pin GPIO_PIN_8
#define KEY_4_GPIO_Port GPIOA
#define KEY_5_Pin GPIO_PIN_9
#define KEY_5_GPIO_Port GPIOA
#define KEY_6_Pin GPIO_PIN_10
#define KEY_6_GPIO_Port GPIOA
#define RF_OUT_Pin GPIO_PIN_10
#define RF_OUT_GPIO_Port GPIOC
#define RF_IN_Pin GPIO_PIN_11
#define RF_IN_GPIO_Port GPIOC
#define RF_IN_EXTI_IRQn EXTI15_10_IRQn
#define SD_CS_Pin GPIO_PIN_2
#define SD_CS_GPIO_Port GPIOD
#define SD_DET_Pin GPIO_PIN_5
#define SD_DET_GPIO_Port GPIOB
#define BEE_CTRL_Pin GPIO_PIN_6
#define BEE_CTRL_GPIO_Port GPIOB
#define RF_CARRIER_Pin GPIO_PIN_7
#define RF_CARRIER_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_usart.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"

#include "stm32g4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "gpio.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint32_t sample_count;
extern __IO uint32_t pulse_filter;
extern __IO uint8_t blink_flag;
extern uint32_t system_sleep ;
extern __IO uint32_t led_on_time;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern __IO uint32_t time_us_count;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INPUT_CAPTURE_Pin GPIO_PIN_2
#define INPUT_CAPTURE_GPIO_Port GPIOE
#define SENSOR_SINGLE_LEVEL_Pin GPIO_PIN_3
#define SENSOR_SINGLE_LEVEL_GPIO_Port GPIOE
#define LED_ON_BOARD_Pin GPIO_PIN_9
#define LED_ON_BOARD_GPIO_Port GPIOF
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_10
#define LED4_GPIO_Port GPIOB
#define KEY1D11_Pin GPIO_PIN_11
#define KEY1D11_GPIO_Port GPIOD
#define KEY0_Pin GPIO_PIN_12
#define KEY0_GPIO_Port GPIOD
#define USER_KEY_Pin GPIO_PIN_15
#define USER_KEY_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_3
#define KEY3_GPIO_Port GPIOD
#define SENSOR_S3_Pin GPIO_PIN_6
#define SENSOR_S3_GPIO_Port GPIOB
#define LIGHT_Pin GPIO_PIN_9
#define LIGHT_GPIO_Port GPIOB
#define SENSOR_S2_Pin GPIO_PIN_1
#define SENSOR_S2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

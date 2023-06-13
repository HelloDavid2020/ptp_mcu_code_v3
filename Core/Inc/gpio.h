/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define GPIO_ON_PB0()          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define GPIO_OFF_PB0()         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define GPIO_TOGGLE_PB0()      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)

#define LED_SYSTEM_ON()          HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET)
#define LED_SYSTEM_OFF()         HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET)
#define LED_SYSTEM_TOGGLE()      HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9)

#define GPIO_ON_PB1()          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define GPIO_OFF_PB1()         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define GPIO_TOGGLE_PB1()      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1)

#define LIGHT_ON()              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define LIGHT_OFF()             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
#define LIGHT_TOGGLE()         HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9)

#define LED3_ON()          HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET)
#define LED3_OFF()         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET)
#define LED3_TOGGLE()      HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15)


#define LED4_ON()          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
#define LED4_OFF()         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define LED4_TOGGLE()      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10)

#define SENSOR_SINGLE_LEVEL    HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)


//    S2  S3  filter
//    L   L   RED
//    L   H   BLUE
//    H   L   WHITE
//    H   H   GREEN

#define SENSOR_S2_HIGH()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET)
#define SENSOR_S3_HIGH()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

#define SENSOR_S2_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET)
#define SENSOR_S3_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)


/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef struct
{
    uint32_t previous;
    uint32_t last ;
//    __IO uint32_t delta_time;
//    __IO uint32_t delay_counting; // 
    __IO uint32_t detect_enable; // 
    __IO uint32_t time_mark; // 
    __IO uint32_t time_start; // 
    __IO uint32_t time_end; // 
    __IO uint32_t time_delay;
    __IO uint32_t sample_cnt;
    __IO uint32_t test_done;
    __IO uint32_t loop_enable;

}gpio_state_t;



extern gpio_state_t sensor_state;
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t read_key0(void);
uint8_t read_key1(void);

void bsp_led_blink(uint32_t interval_time);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


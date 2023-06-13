/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
//#define PULSE_NUMS_HIGH_LIMIT       (2)
//#define PULSE_NUMS_LOW_LIMIT        (5)
#define PULSE_CAPTURE_FILTERCNT     (10)
#define PULSE_CAPTURE_TIMEOUT       (200)     // 100ms

#define PULSE_BUFFER_SIZE            (1024)


extern uint32_t pulse_value;

typedef struct pulse_filter_t
{

    uint32_t pulse_value;
    uint32_t pulse_cnt;

} pulse_filter_t;

typedef struct 
{
    /* data */
    __IO uint32_t enable;

    uint32_t tick_start;  // 1us TIM17
    uint32_t tick_end;    // 1us TIM17
    uint32_t pulse_value;
    
    
    uint32_t pulse_index;

    uint32_t data_ready;     

    
    uint32_t pulse_array[PULSE_BUFFER_SIZE];
    uint32_t pulse_previous;
    uint32_t pulse_last;
    uint32_t pulse_active_cnt;

    pulse_filter_t pulse_filter;
    
    uint32_t timeout;  // systick
    
    
    uint32_t idle_cnt;  // 2Hz
    uint32_t idle_pulse_previous;
    uint32_t idle_pulse_last;

}input_capture_t;

extern input_capture_t input_capture;
extern __IO uint32_t time_us_idle;

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM3_Init(void);
void MX_TIM17_Init(void);

/* USER CODE BEGIN Prototypes */
uint32_t tim_get_period(void);
uint32_t tim_get_frequency(void);
void find_high_light_pulses(input_capture_t * capture);
void capture_start(input_capture_t * capture);
void capture_stop(input_capture_t * capture);
void pulse_data_parser(input_capture_t * capture);
int32_t diff_calculate(uint32_t value_start, uint32_t value_end);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */


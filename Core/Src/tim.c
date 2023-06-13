/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
  
#include <math.h>
#include <stdlib.h>

  
  
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "lcd.h"

/* USER CODE BEGIN 0 */


#define     PRESCALER_VALUE     (uint32_t)(((SystemCoreClock) / 1000000) - 1)

#define     PULSE_DELTA_WINDOW     (50)       // 50us based on TIM3 1us counter

uint32_t freq=20000; 
uint16_t duty=50;
uint32_t pulse_value=0;

__IO uint32_t time_us_idle = 0;

input_capture_t input_capture;
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 170-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  
  
  
  /* USER CODE END TIM3_Init 2 */

}
/* TIM17 init function */
void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM17);

  /* TIM17 interrupt Init */
  NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  TIM_InitStruct.Prescaler = 16;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 9;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM17, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM17);
  /* USER CODE BEGIN TIM17_Init 2 */
//    HAL_TIM_Base_Start_IT(&htim17);
    LL_TIM_EnableCounter(TIM17);
    LL_TIM_EnableIT_UPDATE (TIM17);
  /* USER CODE END TIM17_Init 2 */

}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_icHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PE2     ------> TIM3_CH1
    */
    GPIO_InitStruct.Pin = INPUT_CAPTURE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(INPUT_CAPTURE_GPIO_Port, &GPIO_InitStruct);

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PE2     ------> TIM3_CH1
    */
    HAL_GPIO_DeInit(INPUT_CAPTURE_GPIO_Port, INPUT_CAPTURE_Pin);

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim)
{

}


//static int Is_First_Capture = 0;
//static uint32_t IC_Val1 =0;
//static uint32_t IC_Val2 =0;

static uint32_t period =0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    uint32_t time_idle_diff = 0;
    if(htim->Instance == TIM3)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
//            LED_SYSTEM_TOGGLE();
            LED3_TOGGLE();
            #if 0
            
            if(Is_First_Capture == 0)
            {
              IC_Val1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
                Is_First_Capture =1;
            }
            else
            {             
                IC_Val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
                if(IC_Val2>IC_Val1)
                {
                   period = IC_Val2 - IC_Val1;
                }
                else
                {
                   period = (0xFFFF - IC_Val1) + IC_Val2 + 1;
                }
                
                __HAL_TIM_SET_COUNTER(htim, 0);
                 Is_First_Capture = 0;
            
            }
            
            #endif
            
            // get pulse width
            pulse_value = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1) + 1 ;  // +1 ,start begin 0

            // pulse width storage
            if(input_capture.enable)
            {
							if(pulse_value > 2)
							{
                input_capture.pulse_last =   pulse_value;
                input_capture.pulse_array[input_capture.pulse_index] = input_capture.pulse_last;

                input_capture.pulse_index++;
                if(input_capture.pulse_index > PULSE_BUFFER_SIZE)
                {
                    input_capture.pulse_index  =0;
                    input_capture.enable = 0;
                    input_capture.data_ready =1;
                }
                
                find_high_light_pulses(&input_capture);
							}
            }
            __HAL_TIM_SET_COUNTER(htim, 0);

            //---------------------------------------
            input_capture.idle_pulse_last = time_us_idle;
            time_us_idle =0;
            if(input_capture.idle_pulse_last > 450000 )     // 1Hz
            {
                if( input_capture.idle_pulse_previous >= input_capture.idle_pulse_last )
                    time_idle_diff = input_capture.idle_pulse_previous-input_capture.idle_pulse_last;
                else
                    time_idle_diff = input_capture.idle_pulse_last-input_capture.idle_pulse_previous;

                if(  time_idle_diff < 20000)
                {
                    input_capture.idle_cnt ++;
                    
                }
                else
                {
                    input_capture.idle_cnt = 0;
                }
                
                if(input_capture.idle_cnt > 20 && system_sleep == 0)
                {
                   system_sleep = 1;
                }
                
                input_capture.idle_pulse_previous = input_capture.idle_pulse_last;
            }

            
            if(input_capture.idle_pulse_last < 500 && system_sleep == 1 )     // 500us
            {
                input_capture.idle_cnt = 0;
                system_sleep = 0;
            }
            
            
        }
    }

}



void capture_start(input_capture_t * capture)
{
    printf("Capture Start.\r\n");


    memset(capture, 0x00, sizeof (input_capture_t));
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    
    time_us_count = 0;
    time_us_idle = 0;

    capture->enable = 1;
    capture->tick_start =  time_us_count;
    capture->timeout  = HAL_GetTick();
    LIGHT_ON();


    HAL_Delay(2);
    LIGHT_OFF();

}



void capture_stop(input_capture_t * capture)
{
    printf("Capture End.\r\n\r\n");

    memset(capture, 0x00, sizeof (input_capture_t));
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}

// 1 tick = 10us
void find_high_light_pulses(input_capture_t * capture)
{
	
    int32_t delta =  diff_calculate( capture-> pulse_previous , capture->pulse_last);   

		// 10 KHz -- T=100us, Positive Pulse width = 50us
		//  5 KHz -- T=200us, Positive Pulse width = 100us

    // search lcd highlight time line    
    if( delta < PULSE_DELTA_WINDOW)
    {
        capture->pulse_active_cnt++;
    }
    else
    {
        capture->pulse_active_cnt = 0; 
    }
    
    // find lcd high light
    if(capture->pulse_active_cnt >= pulse_filter)
    {
        capture->enable = 0;
        capture->tick_end =  time_us_count;
        capture->data_ready = 1;
    }
    
    // update pulse value
    capture-> pulse_previous  = capture->pulse_last;

}

void pulse_data_parser(input_capture_t * capture)
{
    
    uint32_t filter_time = 0;
    uint32_t i =0;
    uint32_t time_diff =0;

    // if data is avalible 
    if(capture->data_ready == 1)
    {
        capture->data_ready =0;
        printf("Start timestamp: %d\r\n",capture->tick_start);
        printf("End timestamp  : %d\r\n",capture->tick_end);
        
        
        if(capture->tick_end == 0)
        {
            printf("Invalid capture data\r\n");
            printf("Capture End\r\n");
            printf("----------------------------------------------------------\r\n\r\n");

            return;
        }
        
        
        printf("Pulse Arrays   : %d, ",capture->pulse_index);

        // exclude pulse filter 
        for( i =0; i< capture->pulse_index; i++)
            printf("%d ",capture->pulse_array[i]);
         printf("\r\n");
				
				// calculate filter time
        if(capture->pulse_index > pulse_filter)
        {
            printf("filter time count : %d, ",pulse_filter);

            //  remove ( PULSE_CAPTURE_FILTERCNT+1 ) data
            for(i =0; i <  pulse_filter; i++)
            {
                filter_time += capture->pulse_array[capture->pulse_index-1-i];
                printf("%d ",capture->pulse_array[capture->pulse_index-1-i]);
            }
            printf("\r\n");
        }
				
        printf("filter time= %d us\r\n",filter_time);
				
				
				// calcutlate delta time dT
        time_diff = diff_calculate(capture->tick_start, capture->tick_end);
				
				
        if(time_diff >  filter_time)
        {
						// find first wave edge
            time_diff = time_diff -  filter_time;
            capture_data.real_value = time_diff;
            
            capture_data.sum_value +=  capture_data.real_value;
            
            // search maximum value
            if(capture_data.real_value > capture_data.max_value)
                capture_data.max_value = capture_data.real_value;
            
            // search minimum value
            if(capture_data.min_value == 0)
                capture_data.min_value = 999999;
            
            if(capture_data.real_value < capture_data.min_value )
                capture_data.min_value = capture_data.real_value;
            
            capture_data.sample_cnt ++;
            capture_data.avr_value =   capture_data.sum_value/capture_data.sample_cnt;
            

            
            printf("Time Calculate : %.3f ms\r\n",time_diff/1000.000 );
            printf("Capture End\r\n");
        }
        else
        {
            printf("Invalid Capture\r\n");
            printf("Capture End\r\n");
        }

        printf("----------------------------------------------------------\r\n\r\n");

    }
    
    // time out handler    
    if(capture->enable )
    {
        if( diff_calculate (capture->timeout, HAL_GetTick()) > PULSE_CAPTURE_TIMEOUT) 
        {
            printf("Error Capture Time out.\r\n");
            printf("Capture Stop.\r\n\r\n");               
            capture_stop(capture);

        }
    
    }
    
    
    
}



uint32_t tim_get_period(void)
{
     return period;     // 1 period = 1us
}

uint32_t tim_get_frequency(void)
{
     return (uint32_t)(100000/period);
}
#if 0
uint32_t diff_calculate(uint32_t value_start, uint32_t value_end)
{
    uint32_t diff = 0;
  if(value_end >= value_start )
      diff = value_end - value_start;
  else
      diff = value_end + 0xFFFFFFFF - value_start + 1;
  return diff;
}
#endif

int32_t diff_calculate(uint32_t value_start, uint32_t value_end)
{
    int32_t  diff = (int32_t)value_end - (int32_t)value_start;
 
  return diff;
}
/* USER CODE END 1 */

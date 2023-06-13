/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lib_ringbuffer.h"
#include "lib_commfunc.h"



/* USER CODE END Includes */

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
#define IAP_BUFFER_SIZE             (1024)  // bin mode

typedef void (* callback_rx_t)(uint8_t);
void rx_callback(USART_TypeDef *USARTx, ringbuffer_t *ring_buffer);

extern ringbuffer_t iap_recv_buf;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void uart_rx_handler(void);
void bsp_uart_send_byte(USART_TypeDef *USARTx, uint8_t Byte);
void bsp_uart_send_string(USART_TypeDef *USARTx, const char * str);

void bsp_uart_send_packet(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size);



/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */


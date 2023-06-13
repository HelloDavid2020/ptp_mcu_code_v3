/*
 * board.c
 *
 *  Created on: Nov 23, 2021
 *      Author: David
 */


#include "board.h"
#include "usart.h"


char* build_time(void)
{
    static char time[100]={0};

    #define YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))
    #define MONTH (__DATE__ [2] == 'n' ? (__DATE__[1]=='a' ? 1:5) \
    : __DATE__ [2] == 'b' ? 1 \
    : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 2 : 3) \
    : __DATE__ [2] == 'y' ? 4 \
    : __DATE__ [2] == 'l' ? 6 \
    : __DATE__ [2] == 'g' ? 7 \
    : __DATE__ [2] == 'p' ? 8 \
    : __DATE__ [2] == 't' ? 9 \
    : __DATE__ [2] == 'v' ? 10 : 11)

    #define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 + (__DATE__ [5] - '0'))

    sprintf(time,"%02d/%02d/%02d-%s",YEAR-2000,(MONTH+1),DAY,__TIME__);
    return time;
}



void system_reset(void)
{
  __disable_irq();
  NVIC_SystemReset();
}


void System_DeInit(void)
{
  HAL_RCC_DeInit();
}






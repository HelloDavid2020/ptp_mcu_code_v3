#ifndef    BKP_REGISTER_INTERFACE_H_
#define    BKP_REGISTER_INTERFACE_H_

#include <stdint.h>
#include "main.h"
#include "usart.h"


extern RTC_HandleTypeDef hrtc;


typedef enum {
  RES_OK=0,
  RES_ADDR_ERR, 
  RES_ERR
} bkpStatus;


int8_t bkp_value_write(uint16_t addr, uint16_t value_wr);
bkpStatus bkp_ReadData(uint16_t bkpDR, uint16_t* pDat, uint16_t nLen);
bkpStatus bkp_WriteData(uint16_t bkpDR, uint16_t* pDat, uint16_t nLen);


#endif


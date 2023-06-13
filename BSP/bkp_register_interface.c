#include "main.h"
#include "bkp_register_interface.h"


RTC_HandleTypeDef hrtc;





int8_t bkp_value_write(uint16_t addr, uint16_t value_wr)
{
    uint16_t update_rd = 0;
    
    if(bkp_WriteData(addr, &value_wr, 1) == RES_OK)
    {   
        bkp_ReadData(addr, &update_rd, 1);
        if(update_rd == value_wr)
        {
#if defined (DEBUG)
            printf("-> UPDATE BKP%d VALUE= 0x%4X\r\n",addr,update_rd);
            printf("-> Update succeeded.\r\n\r\n");
#endif
        }
        else
        {
#if defined (DEBUG)
            printf("-> error: bkp write failed.\r\n\r\n");
#endif
            return -1;

        }
    }
    else
    {
        printf("-> error: bkp write failed.\r\n\r\n");
        return -2;

    }
    return 0;
    
}

bkpStatus bkp_WriteData(uint16_t bkpDR, uint16_t* pDat, uint16_t nLen)
{
    uint16_t i=0;
    uint16_t addr=bkpDR;
  
    if(pDat==NULL)
    return RES_ERR;

    __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);


  for(i=0;i<nLen;i++)
  {
    if(IS_RTC_BKP(addr)) /* Check the parameters */
    {
        
      HAL_RTCEx_BKUPWrite(&hrtc, addr, *pDat);

      if( *pDat != HAL_RTCEx_BKUPRead(&hrtc, addr) )
      {
        __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
        return RES_ERR;
      }
      addr++;
      pDat++;
    }
    else
    {
        __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
      return RES_ADDR_ERR;
    }
  }
    __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
    return RES_OK;
}

bkpStatus bkp_ReadData(uint16_t bkpDR, uint16_t* pDat, uint16_t nLen)
{
  uint16_t i=0;
  uint16_t addr=bkpDR;
  
  if(pDat==NULL)
    return RES_ERR;

  
  for(i=0;i<nLen;i++)
  {
    if(IS_RTC_BKP(addr)) /* Check the parameters */
    {
      * pDat=HAL_RTCEx_BKUPRead(&hrtc, addr);
      addr++;
      pDat++;
    }
    else
    {
      return RES_ADDR_ERR;
    }
  }
  return RES_OK;
}

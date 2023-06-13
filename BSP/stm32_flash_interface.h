/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_FLASH_INTERFACE_H
#define STM32_FLASH_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void stm32_flash_jump2address(uint32_t Address);
void stm32_flash_lock(void);

ErrorStatus stm32_flash_write(uint32_t Address, uint8_t *Data, uint32_t DataLength);
void stm32_flash_unlock(void);
ErrorStatus stm32_flash_erase(uint32_t sector, uint32_t sectors_number);

HAL_StatusTypeDef stm32_flash_read(uint32_t addr, uint8_t* pDat, uint32_t NumByteToRead);
ErrorStatus stm32_flash_copy(uint32_t address_destination, uint32_t address_source, uint32_t size);


#endif /* FLASH_INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

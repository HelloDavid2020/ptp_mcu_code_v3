#include "platform.h"
#include "stm32_flash_interface.h"

#include "common_interface.h"
#include "app_openbootloader.h"

//#include "openbl_mem.h"
//
//#include "app_openbootloader.h"
//#include "stm32_flash_interface.h"
//
///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
///* Private function prototypes -----------------------------------------------*/
//static void OPENBL_FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data);
//static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *ListOfPages, uint32_t Length);
//static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void);

///* Exported variables --------------------------------------------------------*/
//OPENBL_MemoryTypeDef FLASH_Descriptor =
//{
//  FLASH_START_ADDRESS,
//  FLASH_END_ADDRESS,
//  (512U * 1024U),
//  FLASH_AREA,
//  OPENBL_FLASH_Read,
//  OPENBL_FLASH_Write,
//  OPENBL_FLASH_SetReadOutProtectionLevel,
//  OPENBL_FLASH_SetWriteProtection,
//  OPENBL_FLASH_JumpToAddress,
//  NULL,
//  OPENBL_FLASH_Erase
//};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the FLASH control register access.
  * @retval None.
  */
void stm32_flash_unlock(void)
{
  HAL_FLASH_Unlock();
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval None.
  */
void stm32_flash_lock(void)
{
  HAL_FLASH_Lock();
}


ErrorStatus stm32_flash_write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
	uint32_t index  = 0U;
	uint32_t length = DataLength;

	uint32_t data_wr = 0;
	uint32_t data_rd = 0;
	ErrorStatus status    = SUCCESS;


  if (length & 7U)
  {
    length = (length & 0xFFFFFFF8U) + 8U;
  }

  /* Unlock the flash memory for write operation */
  stm32_flash_unlock();

  for (index = 0U; index < length; (index += 4U))
  {
	  bsp_board_led_toggle(BSP_LED_1);

	  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	  data_wr = (uint32_t)(*((uint32_t *)((uint32_t)Data + index))) ;

      if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (Address + index), data_wr) != HAL_OK)
      {
		printf("-> ErrorCode= %p\r\n", (void*)HAL_FLASH_GetError());
		printf("-> 0x00000000U No error\r\n");
		printf("-> 0x00000001U Read Protection error\r\n");
		printf("-> 0x00000002U Programming Sequence error\r\n");
		printf("-> 0x00000004U Programming Parallelism error\r\n");
		printf("-> 0x00000008U Programming Alignment error\r\n");
		printf("-> 0x00000010U Write protection error\r\n");
		printf("-> 0x00000020U Operation Error\r\n");

		HAL_FLASH_Lock();
		status    = ERROR;
		return status;
      }

      data_rd = * (uint32_t *)(Address + index);

      if(data_rd != data_wr)
      {
		stm32_flash_lock();

          printf("-> Error: Flash write failed at address %p, ErrorCode= %p\r\n",(void*)(Address + index), (void*)HAL_FLASH_GetError() );
          status    = ERROR;

          return status;
      }
  }

	/* Lock the Flash to disable the flash control register access */
	stm32_flash_lock();
	printf("-> Download in Progress. Address:%p, Len:%ld\r\n",(void*)Address, DataLength);
	status    = SUCCESS;
    return status;

}




/**
* @brief   Read Flash Data
* @retval  FlashStatus
* @details read data from the FLASH.
*/


HAL_StatusTypeDef stm32_flash_read(uint32_t addr, uint8_t* pDat, uint32_t NumByteToRead)
{
  uint32_t i=0;
  uint32_t addr_flash_to_read = addr;

  if(pDat==NULL)
    return HAL_ERROR;


  for(i=0; i<NumByteToRead; i++)
  {
      * pDat++ = *(uint8_t*)(addr_flash_to_read+i);
  }
  return HAL_OK;
}


/**
  * @brief  This function is used to jump to a given address.
  * @param  Address The address where the function will jump.
  * @retval None.
  */
void stm32_flash_jump2address(uint32_t Address)
{
#if 0
  Function_Pointer jump_to_address;

  /* Deinitialize all HW resources used by the Bootloader to their reset values */
  OpenBootloader_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();

#else
  Function_Pointer jump_to_address;

  /* Deinitialize all HW resources used by the Bootloader to their reset values */
  OpenBootloader_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  /* Get the application stack pointer (First entry in the application vector table) */

  /* Get the application entry point (Second entry in the application vector table) */
  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  __set_PSP(*(__IO uint32_t*) Address );  // for rtos
  __set_CONTROL(0);                       // for rtos

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();


#endif




}


ErrorStatus stm32_flash_erase(uint32_t sector, uint32_t sectors_number)
{
	uint32_t sector_error   = 0U;
	uint32_t errors       = 0U;
	ErrorStatus status    = SUCCESS;

	uint32_t StartSector   = sector;

	FLASH_EraseInitTypeDef erase_init_struct;

	/* Unlock the flash memory for erase operation */
	stm32_flash_unlock();
	/* Clear error programming flags */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	erase_init_struct.TypeErase 	= FLASH_TYPEERASE_SECTORS;
	erase_init_struct.Sector   		= StartSector;
	erase_init_struct.NbSectors   	= sectors_number;
	erase_init_struct.Banks = 0;
	erase_init_struct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	printf("-> Erase sector start= %ld\r\n",erase_init_struct.Sector);
	printf("-> Erase number of sectors = %ld\r\n",erase_init_struct.NbSectors);
	printf("-> Erasing...\r\n");

	if (HAL_FLASHEx_Erase(&erase_init_struct, &sector_error) != HAL_OK)
	{
		errors++;
		printf("-> Erase sector error= %ld, %p\r\n",sector_error, (void*)HAL_FLASH_GetError());
	}
	else
	{
		printf("-> Erase done.\r\n");
	}

	/* Lock the Flash to disable the flash control register access */
	stm32_flash_lock();

	if (errors > 0)
	{
		status = ERROR;
		printf("-> Flash Erase ERROR.\r\n");

	}
	else
	{
		status = SUCCESS;
		printf("-> Flash Erase SUCCESS.\r\n");

	}

	return status;
}

// PROGRAM_DOUBLEWORD
#if 0
ErrorStatus stm32_flash_copy(uint32_t address_destination, uint32_t address_source, uint32_t size)
{
	uint32_t index = 0;
	ErrorStatus status    = SUCCESS;

	if (size & 7U)
	{
		size = (size & 0xFFFFFFF8U) + 8U;
	}

	/* Unlock the flash memory for write operation */
	stm32_flash_unlock();

	/* Clear pending flags (if any) */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	bsp_board_led_toggle(BSP_LED_1);

	for (index = 0U; index < size; (index += 8U))
	{

		// step 1:  write data
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address_destination, (uint64_t) (*( (uint64_t *)(address_source) )) ) != HAL_OK)
		{

			printf("-> ErrorCode= %p\r\n", (void*)HAL_FLASH_GetError());
			printf("-> 0x00000000U No error\r\n");
			printf("-> 0x00000001U Read Protection error\r\n");
			printf("-> 0x00000002U Programming Sequence error\r\n");
			printf("-> 0x00000004U Programming Parallelism error\r\n");
			printf("-> 0x00000008U Programming Alignment error\r\n");
			printf("-> 0x00000010U Write protection error\r\n");
			printf("-> 0x00000020U Operation Error\r\n");

			HAL_FLASH_Lock();
			status    = ERROR;
		}

		// step 2:  check data
		// (uint64_t) (*( (uint64_t *)(address_source) ))
//		if (*(uint32_t*)address_destination != *(uint32_t*)address_source)
		if ( (uint64_t) (*( (uint64_t *)(address_destination) )) != (uint64_t) (*( (uint64_t *)(address_source) )))
		{
			stm32_flash_lock();
			printf("-> Error: Flash write failed at address %p, ErrorCode= %p\r\n",(void*)(address_destination), (void*)HAL_FLASH_GetError() );
			status    = ERROR;
		}


		// step 3:  address increment
		address_destination +=8;
		address_source +=8;

		// step 4: feed dog

	}

	/* Lock the Flash to disable the flash control register access */
	stm32_flash_lock();
	status    = SUCCESS;
	return status;
}

#else

ErrorStatus stm32_flash_copy(uint32_t address_destination, uint32_t address_source, uint32_t size)
{
	uint32_t index = 0;
	ErrorStatus status    = SUCCESS;

	if (size & 7U)
	{
		size = (size & 0xFFFFFFF8U) + 8U;
	}

	/* Unlock the flash memory for write operation */
	stm32_flash_unlock();

	/* Clear pending flags (if any) */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	bsp_board_led_toggle(BSP_LED_1);

	for (index = 0U; index < size; (index += 4U))
	{

		// step 1:  write data
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address_destination, *(uint32_t*)address_source ) != HAL_OK)
		{

			printf("-> ErrorCode= %p\r\n", (void*)HAL_FLASH_GetError());
			printf("-> 0x00000000U No error\r\n");
			printf("-> 0x00000001U Read Protection error\r\n");
			printf("-> 0x00000002U Programming Sequence error\r\n");
			printf("-> 0x00000004U Programming Parallelism error\r\n");
			printf("-> 0x00000008U Programming Alignment error\r\n");
			printf("-> 0x00000010U Write protection error\r\n");
			printf("-> 0x00000020U Operation Error\r\n");

			HAL_FLASH_Lock();
			status    = ERROR;
		}

		// step 2:  check data
		if (*(uint32_t*)address_destination != *(uint32_t*)address_source)
		{
			stm32_flash_lock();
			printf("-> Error: Flash write failed at address %p, ErrorCode= %p\r\n",(void*)(address_destination), (void*)HAL_FLASH_GetError() );
			status    = ERROR;
		}


		// step 3:  address increment
		address_destination +=4;
		address_source +=4;

		// step 4: feed dog

	}

	/* Lock the Flash to disable the flash control register access */
	stm32_flash_lock();
	status    = SUCCESS;
	return status;
}

#endif






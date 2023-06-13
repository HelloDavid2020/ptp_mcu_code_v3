#include "bootloader.h"
#include "update_interface.h"
#include "stm32_flash_interface.h"
#include "bkp_register_interface.h"



int8_t boot_flag_write(uint16_t value)
{
	int8_t res = 0;
    res = bkp_value_write(ADDR_BKP_BOOT_FLAG, value);
    return res;
}


uint16_t boot_flag_read(void)
{
	uint16_t value = 0;
    bkp_ReadData(ADDR_BKP_BOOT_FLAG, &value, 1);

    return value;
}


int8_t boot_version_write(uint16_t value)
{
	int8_t res = 0;
#if defined (DEBUG)
    printf("-> Write Boot Flag . %X\r\n",value);
#endif
    res = bkp_value_write(ADDR_BKP_BOOTVER, value);
    return res;
}


void boot_version_read(uint16_t *value)
{
    bkp_ReadData(ADDR_BKP_BOOTVER, value, 1);
#if defined (DEBUG)
    printf("-> Read Boot Flag . %X\r\n",(*value));
#endif

}


//#define BOOT_MODE_APP_NORMAL   	( 0xABCD )	// enter application
//#define BOOT_MODE_DOWNLOAD		( 0x1234 )	// enter boot, download firmware
//#define BOOT_MODE_COPY    		( 0x5678 )	// enter boot, copy firmware image from bankB to bankA
//

void boot_check(void)
{
    uint16_t flag = boot_flag_read();
    printf("-> Boot Flag = 0x%04X\r\n",flag);

    switch(flag)
    {
        case BOOT_MODE_APP_NORMAL:
        	printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
        	stm32_flash_jump2address(ADDRESS_FLASH_APP);

        break;

        case BOOT_MODE_DOWNLOAD:
        	printf("-> Enter Bootloader\r\n");
			update_process_release();

        break;

        case BOOT_MODE_COPY:

    		printf("-> Erasing internal memory sectors [%d %d]\r\n",BANKA_SECTOR_START,BANKA_SECTOR_START+BANKB_SECTORS_NUM);
    		iwdg_feeddog();

    		ErrorStatus status = stm32_flash_erase(BANKA_SECTOR_START, BANKA_SECTORS_NUM);

    		if(status == ERROR)
    		{
    			printf("-> Error: Erasing Failed.\r\n");

    			//---------------------------------------------------------
    				update_process_release();
    			//----------------------------------------------------------
    			return;
    		}
//    		printf("-> Copy Firmware to Application Bank %ld\r\n", firmware_packets_number*256);
    		printf("-> Copy Firmware to Application Bank %ld KB\r\n", 448*1024);

    		status =  stm32_flash_copy( ADDRESS_FLASH_APP, ADDRESS_FLASH_BKP, 448*1024);
    		if(status == SUCCESS)
    		{
    			printf("-> Copy SUCCESS\r\n");
    		}
    		else
    		{
				printf("-> Error: Copy Failed\r\n");
				//---------------------------------------------------------
				update_process_release();
				//----------------------------------------------------------
				return;

    		}


    		printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
    		stm32_flash_jump2address(ADDRESS_FLASH_APP);

        break;

        default:  // BOOT FLAG = 0, if device powered off
        	boot_flag_write(BOOT_MODE_APP_NORMAL);
        	printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
        	stm32_flash_jump2address(ADDRESS_FLASH_APP);
        	break;

    }

}

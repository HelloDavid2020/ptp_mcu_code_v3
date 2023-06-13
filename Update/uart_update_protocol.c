#include "uart_update_protocol.h"
#include "bkp_register_interface.h"
#include "update_interface.h"

bool fw_downlaod_complete = false;



#define UPDATE_IDLE_TIMEOUT	(300000)  // 30 seconds
#define UPDATE_RESP_TIMEOUT	(10000)  // 10 seconds, response timeout

iap_handler_t iap_handle;

static uint8_t data_buffer_write[IAP_BUFFER_SIZE]= {0x00};

static void update_exit(void);
static void update_download_copy(void);
static void update_jump(void);
static void update_fail(void);




static uint32_t time_count_idle = 0;

void iap_init(void)
{
    iap_port_init();
}


void iap_update( iap_handler_t *iap)
{

    ErrorStatus res = 0;

    memset(data_buffer_write, 0x00, sizeof data_buffer_write);
    lib_ascii2hex((void *)iap->rx_buffer,  data_buffer_write, iap->packet_length*2);
    res = stm32_flash_write(ADDRESS_FLASH_APP+iap->data_address, data_buffer_write, iap->packet_length);

    iap->rx_data_length =  iap->rx_data_length + iap->packet_length;
    memset(iap->rx_buffer, 0x00, iap->packet_length*2);
    
    #if 0
    HEX_PRINT( data_buffer_read, iap->packet_length);
    #endif

    if(res == ERROR)
    {    
        iap->process_state = IAP_END;
        printf("FLASH WIRITE FAILED.\r\n");
    }
    else
    {
        iap->data_address += iap->packet_length;
        printf("FLASH WIRITE SUCCESSED. %ld bytes.\r\n",iap->packet_length);
    }
}



void iap_process_handler(iap_handler_t *iap)
{
    char rsp_buff[128]={0};
//    static uint32_t time_count_quit = 0;
//    signature_infor_t sign_info_temp;
    if(UPDATE_CAN == update_process_busy())
    {
    	return;
    }


   switch (iap->process_state)
    {
        case IAP_START: 
            iap->checksum =0;
            iap->rx_data_length = 0;      
            iap->process_state =  IAP_WAIT;
            iap->time_out = HAL_GetTick();
        
/*
//            sign_info.application_size = iap_handle.file_size;
//            sign_info.application_crc = iap_handle.file_crc16_chksum;
//             printf("-> sign_info to write\r\n");
//            printf("-> sign_info.application_size= %d\r\n",sign_info.application_size);
//            printf("-> sign_info.application_crc = %d\r\n\r\n",sign_info.application_crc);
//            printf("-> sign_info.application_addr = 0x%08X\r\n\r\n",FLASH_ADDRESS_APPLICATION + sign_info.application_size);
//
////            signature_write(&sign_info);
////            signature_read(&sign_info_temp);
//            printf("-> sign_info to read\r\n");
//
//            printf("-> sign_info.application_size= %d\r\n",sign_info_temp.application_size);
//            printf("-> sign_info.application_crc = %d, 0x%08X\r\n",sign_info_temp.application_crc, sign_info_temp.application_crc);
//
//
*/
            stm32_flash_erase(BANKA_SECTOR_START, BANKA_SECTORS_NUM);

        
            sprintf(rsp_buff,"+IAPSTART=%ld,0\r\n",iap->targetID);
            iap_send(rsp_buff);

        break;

        case IAP_DATA: 
  
            iap_update(iap);
            printf("IAP DATA SIZE:%ld Bytes, %ld KB\r\n\r\n",iap->rx_data_length,iap->rx_data_length/1024);
          
            iap->process_state =  IAP_WAIT;
            iap->time_out = HAL_GetTick();            
            HAL_Delay(1);

            sprintf(rsp_buff,"+IAPDATA=%ld,%ld,0\r\n\r\n",iap->targetID, iap->packet_index);
            iap_send(rsp_buff);

        break;

        case IAP_END:  
            iap_send("+IAP=END\r\n");
            iap_send("OK\r\n");
            iap->process_state =  IAP_CHECK;
            iap->checksum =~iap->checksum;
            printf("checksum= %ld, %p\r\n",iap->checksum, (void*)iap->checksum);
            iap->time_out = HAL_GetTick();


        break;


        case IAP_CHECK:

        	update_jump();

        	break;

        case IAP_FAIL: 
			update_fail();


			break;

        case IAP_WAIT: 
            if( HAL_GetTick()-iap->time_out > UPDATE_RESP_TIMEOUT)  //  10 seconds
            {
                iap->time_out =HAL_GetTick();
                iap->process_state =IAP_IDLE;
            }
        break; 


        case IAP_IDLE: 
            if( HAL_GetTick()-time_count_idle > 1000)  //  1 seconds
            {
                time_count_idle =HAL_GetTick();
                iap_send("+IAP=IDLE\r\n");

            }
            if( HAL_GetTick() - iap_handle.time_out > UPDATE_IDLE_TIMEOUT)
            {

            	update_exit();
//            	if(signature_check()==PASSED)
//                {
//                    update_successed_handle();
//                }
//                else
//                {
//                    printf("-> Firmware bin damage.\r\n");
//                    printf("-> RESET.\r\n");
//                    system_reset();
//                }

                
            }
            
            
        break;   

        case IAP_STANDBY:
			if( HAL_GetTick()-time_count_idle > 1000)  //  1 seconds
			{
				time_count_idle =HAL_GetTick();
				iap_send("+IAP=STANDBY\r\n");
			}

        	break;

        default:  
        break; 

    }



}

// NO COPY, JUST JUMP!!!
// NO COPY, JUST JUMP!!!
// NO COPY, JUST JUMP!!!

static void update_download_copy(void)
{
#if 0  // NO COPY, JUST JUMP

	printf("-> Erasing internal memory sectors [%d %d]\r\n",BANKA_SECTOR_START,BANKA_SECTOR_START+BANKB_SECTORS_NUM);
	iwdg_feeddog();

	ErrorStatus status = stm32_flash_erase(BANKA_SECTOR_START, BANKA_SECTORS_NUM);

	if(status == ERROR)
	{
		printf("-> Error: Erasing Failed.\r\n");
		return;
	}
	printf("-> Copy Firmware to Application Bank %ld\r\n", iap_handle.rx_data_length);

	status =  stm32_flash_copy( ADDRESS_FLASH_APP, ADDRESS_FLASH_BKP, iap_handle.rx_data_length);
	if(status == SUCCESS)
	{
		printf("-> Copy SUCCESS\r\n");
	}
	else
	{
		printf("-> Error: Copy Failed\r\n");
		return;
	}
#endif
	boot_flag_write(BOOT_MODE_APP_NORMAL);
	printf("-> Update Completed. \r\n") ;
	printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
	HAL_Delay(1); // wait for printf
	stm32_flash_jump2address(ADDRESS_FLASH_APP);


}

static void update_jump(void)
{
	printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
	stm32_flash_jump2address(ADDRESS_FLASH_APP);
}

// condition:
static void update_exit(void)
{


#if 0  // NO COPY, JUST JUMP

	printf("-> Erasing internal memory sectors [%d %d]\r\n",BANKA_SECTOR_START,BANKA_SECTOR_START+BANKB_SECTORS_NUM);
	iwdg_feeddog();

	ErrorStatus status = stm32_flash_erase(BANKA_SECTOR_START, BANKA_SECTORS_NUM);

	if(status == ERROR)
	{
		printf("-> Error: Erasing Failed.\r\n");
		return;
	}
	printf("-> Copy Firmware to Application Bank %ld\r\n", iap_handle.rx_data_length);

	status =  stm32_flash_copy( ADDRESS_FLASH_APP, ADDRESS_FLASH_BKP, iap_handle.rx_data_length);
	if(status == SUCCESS)
	{
		printf("-> Copy SUCCESS\r\n");
	}
	else
	{
		printf("-> Error: Copy Failed\r\n");
		return;
	}
#endif

	boot_flag_write(BOOT_MODE_APP_NORMAL);
    printf("-> Exit Bootloader mode. \r\n") ;
	printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
    HAL_Delay(1); // wait for printf

	stm32_flash_jump2address(ADDRESS_FLASH_APP);
}

void update_fail(void)
{
    printf("-> Update Failed. \r\n") ;
    printf("-> Reinit  iap_handle \r\n") ;



	memset(&iap_handle,0x00,sizeof iap_handle);
	iap_handle.process_state = IAP_IDLE;
	time_count_idle = 0;

//---------------------------------------------------------
	update_process_release();
//----------------------------------------------------------


#if 0

	boot_flag_write(BOOT_MODE_APP_NORMAL);
    printf("-> Exit Bootloader mode. \r\n") ;
	printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
    HAL_Delay(1); // wait for printf

	stm32_flash_jump2address(ADDRESS_FLASH_APP);
#endif
}

#if 0
static void update_standby(void)
{
	memset(&iap_handle,0x00,sizeof iap_handle);
	iap_handle.process_state = IAP_STANDBY;
	time_count_idle = 0;

}

#endif



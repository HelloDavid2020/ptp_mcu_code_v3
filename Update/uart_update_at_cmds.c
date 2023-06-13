#include "uart_update_at_cmds.h"
#include "update_interface.h"


static void at_cmd_parse(uint8_t *cmd );
static void at_iap_start( char* cmd ) ;
static void at_iap_data( char* cmd );
static void at_iap_end( char* cmd );
static void at_iap_abort( char* cmd );
static void at_ok( char* cmd );
static void at_erase( char* cmd );
static void at_read( char* cmd );
static void at_write( char* cmd );
static void at_reset( char* cmd );
static void at_info( char* cmd );
static void at_boot( char* cmd );

const at_cmd_items_t at_command_items[] =
{
    // UPGRADE

    {"AT+IAPSTART",         at_iap_start},
    {"AT+IAPDATA",          at_iap_data},
    {"AT+IAPEND",           at_iap_end},
    {"AT+IAPABORT",         at_iap_abort},
    {"AT+OK",         		at_ok},
    {"AT+ERASE",         	at_erase},
    {"AT+READ",         	at_read},
    {"AT+WRITE",       		at_write},
    {"AT+RESET",       		at_reset},
    {"AT+INFO",       		at_info},
    {"AT+BOOT",       		at_boot},


};

static void at_ok( char* cmd )
{
	printf("OK\r\n");
}

static void at_iap_start( char* cmd )
{
    char *p = strstr(cmd,"=");  

//---------------------------------------------------------
	if(update_process_busy() != UPDATE_RELEASE) // idle
	{
		printf("WARNING: UPATE PROCSS IS BUSY!\r\n");
		return;
	}
	update_process_take(UPDATE_UART);
//----------------------------------------------------------




    if(p)
    {
        //AT+IAPSTART=4,10340,A,256,41311,PassthruPackage.bin
        char *cmd_target =strstr(cmd,"AT+IAPSTART");
        
        if(sscanf((void*)cmd_target,"AT+IAPSTART=%lu,%lu,%c,%lu,%lu,%s",&(iap_handle.targetID), &iap_handle.file_size, &iap_handle.transfer_mode, &iap_handle.buffer_size,  &iap_handle.file_crc16_chksum, iap_handle.file_name)==6)
        {
            switch(iap_handle.targetID)
            {
                case TARGET_MCU:
                    {   
                        iap_handle.process_state = IAP_START;
                        iap_handle.data_address  = 0;
                    }
                    break;
                
                
               
                
                default: 
                     printf("UNKOWN TARGET\r\n");
                    break;
            }
        }
        else
        {
            memset(&iap_handle, 0x00, sizeof(iap_handler_t));
             printf("INVALID PARA.\r\n");
        }
     }    
}


static void at_iap_data( char* cmd )
{

    char *p = strstr(cmd,"=");  
    if(p)
    {
        // AT+IAPDATA=5,1,256,62628,52494646C6980000........
        // AT+IAPDATA=4,1,256,63132,
        char *cmd_target =strstr(cmd,"AT+IAPDATA");

        if(sscanf((char*)cmd_target,"AT+IAPDATA=%*d,%ld,%ld,%ld,%s",&iap_handle.packet_index, &iap_handle.packet_length, &iap_handle.packet_crc16_chksum,  iap_handle.rx_buffer)==4)
        {
            iap_handle.process_state = IAP_DATA;

             printf("IAP PACKET INDEX : %ld\r\n",iap_handle.packet_index);
             printf("IAP PACKET LENGTH: %ld\r\n",iap_handle.packet_length);
             printf("IAP PACKET CRC16 : %ld\r\n",iap_handle.packet_crc16_chksum);
             printf("\r\n");
        }
        else
        {
             printf("INVALID PARA.\r\n");
        }
     }    
}


static void at_iap_end( char* cmd )
{
    iap_handle.process_state = IAP_END;
}



static void at_iap_abort( char* cmd )
{
    iap_handle.process_state = IAP_ABORT;
}


static void at_erase( char* cmd )
{
	uint32_t sector = 0;
	uint32_t sectors_num = 0;
    char *p = strstr(cmd,"AT+ERASE=");
    if(p)
    {
        char *cmd_target =strstr(cmd,"AT+ERASE");

        if(sscanf((char*)cmd_target,"AT+ERASE=%ld,%ld",&sector, &sectors_num) == 2)
        {

			printf("-> Erase flash sector index %ld, sectors number %ld\r\n",sector,sectors_num);
			stm32_flash_erase(sector, sectors_num);
        }
        else
        {
             printf("-> Error: Invalid Data.\r\n");
        }
     }

}


static void at_read( char* cmd )
{
	uint32_t addr = 0;
	uint32_t len = 0;

	uint8_t read_buff_hex[64]={0};


	char *p = strstr(cmd,"AT+READ=");
	if(p)
	{
		char *cmd_target =strstr(cmd,"AT+READ=");

		if(sscanf((char*)cmd_target,"AT+READ=%p,%ld",(void**)&addr, &len ) == 2)
		{
			if(len <= 64)
			{
				stm32_flash_read(addr,read_buff_hex, len);

				printf("-> Data to Readback %p, ", (void**)addr);
				hex_print( (void*)read_buff_hex, len);
				printf("\r\n");
			}
			else
			{
				printf("-> Error: The maximum data length is 64 bytes.\r\n");
			}

		}
		else
		{
			printf("-> Error: Invalid Data.\r\n");
		}
	 }



}
static void at_reset( char* cmd )
{
	printf("-> System Reset...\r\n");
	system_reset();

}

static void at_info( char* cmd )
{

    printf("===================================\r\n");
    printf("-> BOOTLOADER\r\n");
    printf("-> BootVersion %d\r\n",boot_flag_read());
    printf("-> ProductID= SN123456678\r\n");
    printf("-> Device ID= %p\r\n",(void*)HAL_GetDEVID());
    printf("-> HardwaeID= 0x%02X\r\n",hardware_pcba.id);
    printf("-> BuildTime= %s\r\n",build_time());

}


static void at_boot( char* cmd )
{
    printf("-> Jump to Bootloader\r\n");

	stm32_flash_jump2address(ADDRESS_FLASH_BOOT);
}


static void at_write( char* cmd )
{
	uint32_t addr = 0;
	uint32_t len = 0;

	uint8_t write_buff[128]={0};
	uint8_t read_buff[128]={0};
	uint8_t write_buff_hex[64]={0};


	char *p = strstr(cmd,"AT+WRITE=");
	if(p)
	{
		char *cmd_target =strstr(cmd,"AT+WRITE=");

		if(sscanf((char*)cmd_target,"AT+WRITE=%p,%ld,%s",(void**)&addr, &len, write_buff)==3)
		{

			if(len <= 64)

		    {
				lib_ascii2hex((void*)write_buff, (void*)write_buff_hex, len*2);
				printf("-> Data to Write %p, %ld ",(void*)addr, len);
				hex_print( (void*)write_buff_hex, len);
				stm32_flash_write(addr, write_buff_hex, len);


				stm32_flash_read(addr,read_buff,len);
				printf("-> Data to Readback %p, %ld ", (void*)addr, len);
				hex_print( (void*)read_buff, len);
				printf("\r\n");
		    }
			else
			{
				printf("-> Error: The maximum data length is 64 bytes.\r\n");
			}

		}
		else
		{
			printf("-> Error: Invalid Data.\r\n");
		}
	 }

}



static void at_cmd_parse(uint8_t *cmd )
{
    int32_t total_items = sizeof( at_command_items ) / sizeof( at_cmd_items_t );
    char *  ptr;

    if(cmd == NULL) 
        return;

    for( int32_t i = 0; i<total_items; i++ )
    {
        if(strlen((char*)cmd)>2)
        {
            ptr = strstr((char*)cmd, at_command_items[i].p_at_command);    
            if( ptr )
            {
                at_command_items[i].pAtFunc( (char*)cmd );
                return;
            }
        }
    }
//    DBG_TRACE("UNKOWN COMMAND\r\nERROR\r\n");
}


static at_cmd_buffer_t at_commands;

void at_cmd_recv_handle( ringbuffer_t * input_cmd )
{

    while( lib_ringbuf_read_byte(input_cmd, &at_commands.rx_buff[at_commands.rx_index]))
    {
        at_commands.rx_index ++;

        if(at_commands.rx_index >2)
        {
            if( at_commands.rx_buff[at_commands.rx_index-2] == 0x0d && at_commands.rx_buff[at_commands.rx_index-1] == 0x0a  )
            {
               
                at_commands.rx_buff[at_commands.rx_index - 2] = '\0';   
                
                    at_cmd_parse( at_commands.rx_buff);

                memset(at_commands.rx_buff, 0x00, sizeof at_commands.rx_buff);
                at_commands.rx_index = 0;
            }
        }


        if( at_commands.rx_index >= CMD_LENGTH ) at_commands.rx_index =0;
    }
}



#include "can_update_protocol.h"
#include "lib_commfunc.h"
#include "stm32_flash_interface.h"
#include "update_interface.h"

can_update_handler_t can_update_handle;
can_update_data_t can_update_data;


static uint32_t can_update_event = 0;

void can_update_init(void)
{
//    iap_port_init();
}




void can_respond(uint32_t can_id, uint32_t ack)
{
	uint8_t buffer[1]={0x00};
	buffer[0]=ack;
	can_send_msg(&hcan1, can_id, buffer, 1);

}

void can_respond_version(uint32_t boot_version)
{
	uint8_t buffer[8]={0x00};
	uint32_t dev_id =HAL_GetDEVID();

	buffer[0] = CAN_RSP_ACK;
	can_send_msg(&hcan1, CANID_GETVERSION, buffer, 1);


	buffer[0] = boot_version & 0xFF;
	can_send_msg(&hcan1, CANID_GETVERSION, buffer, 1);


	buffer[0] = (dev_id>>8) & 0xFF;
	buffer[1] = dev_id & 0xFF;

	can_send_msg(&hcan1, CANID_GETVERSION, buffer, 2);

	buffer[0] = CAN_RSP_ACK;
	can_send_msg(&hcan1, CANID_GETVERSION, buffer, 1);


}


// product id
void can_respond_pid(uint8_t* pid)
{
	uint8_t buffer[8]={0x00};
	uint8_t pid_hex[6]={0x00};
	uint8_t *ptr = NULL;

	buffer[0] = CAN_RSP_ACK;
	can_send_msg(&hcan1, CANID_GETID, buffer, 1);

//	HAL读取STM32 96位全球唯一ID(UID)
//	HAL_GetUID(uint32_t *UID)
	lib_ascii2hex(pid, pid_hex, 12);
	ptr = pid_hex;

	buffer[0] = 'S';
	buffer[1] = 'N';
	buffer[2] = (* ptr++);
	buffer[3] = (* ptr++);
	buffer[4] = (* ptr++);
	buffer[5] = (* ptr++);
	buffer[6] = (* ptr++);
	buffer[7] = (* ptr++);

	can_send_msg(&hcan1, CANID_GETID, buffer, 8);

	buffer[0] = CAN_RSP_ACK;
	can_send_msg(&hcan1, CANID_GETID, buffer, 1);


}


void can_event_send(update_event_t evt)
{
	can_update_event |= evt;
}

void can_event_get(update_event_t * evt)
{
	* evt = can_update_event;
}


void can_event_clear(void)
{
	can_update_event= 0;
}




void can_event_handle(void)
{
	static uint32_t firmware_packets_number = 0;
	static uint32_t address_offset = 0;

	update_event_t evt;
    if(UPDATE_UART == update_process_busy())
    {
    	return;
    }

	can_event_get(&evt);


	if(evt == 0)
		return;


	if(evt & EVT_GETVERSION)
	{
		printf("EVT: GETVERSION\r\n");
		can_respond_version(0x10);

	}
	if(evt & EVT_GETID)
	{
		printf("EVT: GETID\r\n");
		can_respond_pid((void*)"1234567890AB");
	}
	if(evt & EVT_ERASE )
	{
		printf("EVT: ERASE\r\n");
		printf("-> Erasing internal memory sectors [%d %d]\r\n",BANKB_SECTOR_START,BANKB_SECTOR_START+BANKB_SECTORS_NUM);
		iwdg_feeddog();

		ErrorStatus status = stm32_flash_erase(BANKB_SECTOR_START, BANKB_SECTORS_NUM);
		if(status == SUCCESS)
			can_respond(CANID_ERASE, CAN_RSP_ACK);
		else
			can_respond(CANID_ERASE, CAN_RSP_NACK);


	}
	if(evt & EVT_WRITE )
	{
		if( UPDATE_RELEASE != update_process_busy()) // idle
		{
			printf("WARNING: UPATE PROCSS IS BUSY!\r\n");
			return;
		}

		update_process_take(UPDATE_CAN);



		firmware_packets_number = 0;
		printf("EVT: EVT_WRITE\r\n");

		can_update_data.address_start = ADDRESS_FLASH_BKP; // 0x08080000
		address_offset = 0;

		printf("Download in Progress:  Address 0x%02X%02X%02X%02X, Length %d\r\n",
					can_update_data.frame_write.data[0],
					can_update_data.frame_write.data[1],
					can_update_data.frame_write.data[2],
					can_update_data.frame_write.data[3],
					(can_update_data.frame_write.data[4]+1));

		memset(&can_update_data.frame_write ,0x00, sizeof can_update_data.frame_write );
		can_respond(CANID_WRITE, CAN_RSP_ACK);
	}

	if(evt & EVT_DOWNLOAD )
	{
		firmware_packets_number++;
		printf("EVT: EVT_DOWNLOAD %ld \r\n",firmware_packets_number);


		// step1 pack can frames data
		memset(&can_update_data.packets_data ,0x00, sizeof can_update_data.packets_data );
		for(int i = 0; i< 32; i++)
		{
			can_update_data.packets_data[0 + i*8] = can_update_data.frame_download[i].data[0];
			can_update_data.packets_data[1 + i*8] = can_update_data.frame_download[i].data[1];
			can_update_data.packets_data[2 + i*8] = can_update_data.frame_download[i].data[2];
			can_update_data.packets_data[3 + i*8] = can_update_data.frame_download[i].data[3];
			can_update_data.packets_data[4 + i*8] = can_update_data.frame_download[i].data[4];
			can_update_data.packets_data[5 + i*8] = can_update_data.frame_download[i].data[5];
			can_update_data.packets_data[6 + i*8] = can_update_data.frame_download[i].data[6];
			can_update_data.packets_data[7 + i*8] = can_update_data.frame_download[i].data[7];
		}

		// clear CAN DATA frame index and rx buffer
		memset(&can_update_data.frame_download ,0x00, sizeof can_update_data.frame_download );
	    can_update_data.frames_index = 0;


		// step2 data to write
		stm32_flash_write(can_update_data.address_start + address_offset, can_update_data.packets_data, 256);
		address_offset +=256;

		hex_print_16bytes((void*)can_update_data.packets_data, 256);



		can_respond(CANID_DOWNLOD, CAN_RSP_ACK);

	}
	if(evt & EVT_END )
	{
		printf("EVT: EVT_END\r\n");

		printf("-> Erasing internal memory sectors [%d %d]\r\n",BANKA_SECTOR_START,BANKA_SECTOR_START+BANKB_SECTORS_NUM);
		iwdg_feeddog();

		ErrorStatus status = stm32_flash_erase(BANKA_SECTOR_START, BANKA_SECTORS_NUM);

		if(status == ERROR)
		{
			can_respond(CANID_GO, CAN_RSP_NACK);
			printf("-> Error: Erasing Failed.\r\n");

			//---------------------------------------------------------
				update_process_release();
			//----------------------------------------------------------


			return;
		}
		printf("-> Copy Firmware to Application Bank %ld\r\n", firmware_packets_number*256);

		status =  stm32_flash_copy( ADDRESS_FLASH_APP, ADDRESS_FLASH_BKP, firmware_packets_number*256);
		if(status == SUCCESS)
		{
	 		can_respond(CANID_GO, CAN_RSP_ACK);
			printf("-> Copy SUCCESS\r\n");
		}
		else
		{
	 		can_respond(CANID_GO, CAN_RSP_NACK);
			printf("-> Error: Copy Failed\r\n");
			//---------------------------------------------------------
				update_process_release();
			//----------------------------------------------------------
			return;

		}
		printf("-> Jump to Appliaction %p\r\n",(void*)ADDRESS_FLASH_APP);
		stm32_flash_jump2address(ADDRESS_FLASH_APP);



	}
	if(evt & EVT_RESET )
	{
		can_respond(CANID_GO, CAN_RSP_ACK);
		printf("EVT: EVT_RESET\r\n");

		printf("-> System RESET......\r\n\r\n\r\n");
		system_reset();

	}
	if(evt & EVT_BOOT )
	{
		can_respond(CANID_GO, CAN_RSP_ACK);
		printf("EVT: EVT_BOOT\r\n");
		printf("-> Enter Boot mode...\r\n\r\n\r\n");

	}



	can_event_clear();
}



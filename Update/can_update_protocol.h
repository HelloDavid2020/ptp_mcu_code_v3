#ifndef _IAP_CAN_PROTOCOL_H_
#define _IAP_CAN_PROTOCOL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "update_interface.h"


#define CAN_RX_BUFFER_SIZE      (256)


#define CANID_GET				(0x00)
#define CANID_GETVERSION		(0x01)
#define CANID_GETID				(0x02)
#define CANID_SPEED				(0x03)
#define CANID_DOWNLOD			(0x04)
#define CANID_READ				(0x11)
#define CANID_GO				(0x21)  // reset
#define CANID_WRITE				(0x31)
#define CANID_ERASE				(0x43)
#define CANID_WRITE_PROTECT		(0x63)
#define CANID_WRITE_UNPROTECT	(0x73)
#define CANID_READ_PROTECT		(0x82)
#define CANID_READ_UNPROTECT	(0x92)

#define CAN_RSP_ACK				(0x79) // OK
#define CAN_RSP_NACK			(0x1F) // ERROR


//#define BANKA_SECTOR_START 	4
//#define BANKA_SECTORS_NUM  	4
//
//#define BANKB_SECTOR_START 	8
//#define BANKB_SECTORS_NUM  	4
//
//#define ADDRESS_FLASH_BOOT 	(0x08000000)
//#define ADDRESS_FLASH_APP 	(0x08010000)
//#define ADDRESS_FLASH_BKP 	(0x08080000)


typedef void (* callback_can_rx_t)(uint8_t);
void can_rx_callback(USART_TypeDef *USARTx, ringbuffer_t *ring_buffer);



typedef enum
{
	UPDATE_ACK		=0,
	UPDATE_NACK 	=1,
	UPDATE_TIMEOUT,
	UPDATE_CHECK_FAIL,
} update_result_t;


typedef enum
{
    UPDATE_IDLE            =0,
	UPDATE_START           =1,
	UPDATE_DATA            =2,
	UPDATE_VERIFY          =3,
	UPDATE_END             =4,
	UPDATE_ABORT           =5,
	UPDATE_PASS            =6,
	UPDATE_FAIL            =7,
	UPDATE_QUIT            =8,
	UPDATE_VERSION_CHECK   =9,
	UPDATE_WAIT            =10
} update_state_t;



typedef enum
{
	EVT_GETVERSION 		= (1<<0),
	EVT_GETID     		= (1<<1),
    EVT_ERASE      		= (1<<2),
    EVT_WRITE 			= (1<<3),
    EVT_DOWNLOAD		= (1<<4),
    EVT_END				= (1<<5),

    EVT_RESET   		= (1<<17), // CANID_GO
    EVT_BOOT   			= (1<<18), // CANID_GO

} update_event_t;

typedef struct
{
    char        file_name[32];
    uint32_t    file_size;
    uint32_t    buffer_size;
    uint32_t    data_address;

    uint32_t    packet_index;  
    uint32_t    packet_length;
    uint32_t    packet_total_cnt;
    uint32_t    packet_crc16_chksum; 

    uint32_t    checksum;  // checksum

    uint32_t    audio_index;
    update_state_t    process_state;

    uint32_t    time_out;  // commuacation time out
    uint8_t     ack;  // commuacation time out
    uint8_t     cmd_index;  // 
    uint8_t     retry_times;  // 

}can_update_handler_t;


typedef struct {
  CAN_RxHeaderTypeDef header;
  uint8_t  data[8];
} can_rx_message_t;

typedef struct {
  CAN_TxHeaderTypeDef header;
  uint8_t  data[8];
} can_tx_message_t;


typedef struct
{
	uint32_t id;
	uint32_t address_start;
	uint32_t address_offset;

	can_rx_message_t 	frame_write; 			// canid=0x31
	can_rx_message_t 	frame_download[32]; 	// canid=0x04
	uint32_t         	frames_index; 	   		// canid =0x04
	uint32_t			packets_index;    		// canid =0x31+0x04
	uint8_t				packets_data[CAN_RX_BUFFER_SIZE];

}can_update_data_t;

extern can_update_data_t can_update_data;

extern can_update_handler_t can_update_handle;
extern void can_update_process_handler(can_update_handler_t *iap);
extern void can_update_init(void);

void can_event_send(update_event_t evt);
void can_event_get(update_event_t * evt);
void can_event_handle(void);
void can_event_clear(void);

void can_respond(uint32_t can_id, uint32_t ack);
void can_respond_version(uint32_t boot_version);
void can_respond_pid(uint8_t* pid);


#ifdef __cplusplus
}
#endif

#endif 

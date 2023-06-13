#ifndef _IAP_PROTOCOL_H_
#define _IAP_PROTOCOL_H_

#include "update_interface.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef void (* callback_rx_t)(uint8_t);
void rx_callback(USART_TypeDef *USARTx, ringbuffer_t *ring_buffer);


#define IAP_BUFFER_SIZE             (1024)  // bin mode



typedef enum
{
    TARGET_MCU      = 0,

} iap_target_id_t;

typedef enum
{
    IAP_OK               =0,
    IAP_ERR_HW           =1,
    IAP_ERR_SIGNATURE    =2,
    IAP_ERR_FILENAME     =3,
    IAP_ERR_TIMEOUT      =4,
    IAP_ERR_HW_NO_MATH   =5,
    IAP_ERR_MD5          =6,
    IAP_ERR_MD5LEN       =7,
    IAP_ERR_NOACK        =8
} iap_update_result_t;



typedef enum
{
    IAP_IDLE            =0,
    IAP_START           =1,
    IAP_DATA            =2,
    IAP_END             ,
    IAP_ABORT           ,
    IAP_CHECK           ,
    IAP_FAIL            ,
    IAP_WAIT            ,
    IAP_STANDBY         ,

} iap_process_state_t;

typedef struct
{
    uint32_t    targetID;
    char        file_name[32];
    uint32_t    file_size;
    char        transfer_mode;
    uint32_t    buffer_size;
    uint32_t    data_address;

    uint8_t     rx_buffer[IAP_BUFFER_SIZE*2];
    uint32_t    rx_data_length;
    uint32_t    tx_data_length;

    uint32_t    packet_index;  
    uint32_t    packet_length;
    uint32_t    packet_total_cnt;
    uint32_t    packet_crc16_chksum; 

    uint32_t    file_crc16_chksum;  // file data checksum
    uint32_t    checksum;  // checksum

    uint32_t    audio_index;
    iap_process_state_t    process_state;

    uint32_t    time_out;  // commuacation time out
    uint8_t     ack;  // commuacation time out
    uint8_t     cmd_index;  // 
    uint8_t     retry_times;  // 

    uint8_t target_version[10];   // 
    uint8_t result;
}iap_handler_t;


extern iap_handler_t iap_handle;
extern void iap_process_handler(iap_handler_t *iap);
extern void iap_init(void);
void update_successed_handle(void);

#ifdef __cplusplus
}
#endif

#endif 

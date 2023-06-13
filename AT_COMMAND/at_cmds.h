#ifndef _AT_CMDS_H__
#define _AT_CMDS_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

#define  CMD_LENGTH (1024*2+256)

typedef void (* at_cmd_function)(char*);

typedef struct 
{
    char* p_at_command;
    at_cmd_function pAtFunc;
}at_cmd_items_t;


typedef struct
{
    uint8_t  rx_buff[CMD_LENGTH];
    uint32_t rx_index;
}at_cmd_buffer_t;



typedef enum
{
    AT_CMD_CONTROL = 1,
    AT_CMD_SET,
    AT_CMD_GET,
} at_cmd_type_t;

typedef struct 
{
    at_cmd_type_t command;
    char tag[32];
    char data[128];
    char ack_data[128];
    char reponse[288];

}at_cmd_paser_t;



typedef enum
{
    CMD_DEV_CDC = 1,
    CMD_DEV_WIFI,
    CMD_DEV_DEBUG,
} target_device_t;


extern target_device_t at_cmd_form;

at_cmd_paser_t at_command_parser( const char *cmd);
void at_cmd_recv_handle( ringbuffer_t * input_cmd );


#ifdef __cplusplus
}
#endif

#endif 


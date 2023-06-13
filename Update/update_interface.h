#ifndef _UPDATE_INTERFACE_H_
#define _UPDATE_INTERFACE_H_



#ifdef __cplusplus
extern "C" {
#endif



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>


#include "main.h"

#include "lib_ringbuffer.h"
#include "uart_update_port.h"
#include "uart_update_at_cmds.h"
#include "uart_update_protocol.h"
#include "lib_commfunc.h"
#include "bootloader.h"


#define BANKA_SECTOR_START 	4
#define BANKA_SECTORS_NUM  	4

#define BANKB_SECTOR_START 	8
#define BANKB_SECTORS_NUM  	4

#define ADDRESS_FLASH_BOOT 	(0x08000000)
#define ADDRESS_FLASH_APP 	(0x08010000)
#define ADDRESS_FLASH_BKP 	(0x08080000)

typedef enum{
	UPDATE_RELEASE=0,
	UPDATE_CAN,
	UPDATE_UART
}process_state_t;

process_state_t update_process_busy(void);
void update_process_release();
void update_process_take(process_state_t state);



#ifdef __cplusplus
}
#endif

#endif 

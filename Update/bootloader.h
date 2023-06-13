#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include "platform.h"


#ifdef __cplusplus
extern "C" {
#endif




#define BOOT_MODE_APP_NORMAL   	( 0xABCD )	// enter application
#define BOOT_MODE_DOWNLOAD		( 0x1234 )	// enter boot, download firmware
#define BOOT_MODE_COPY    		( 0x5678 )	// enter boot, copy firmware image from bankB to bankA


#define ADDR_BKP_BOOT_FLAG		( RTC_BKP_DR0 )
#define ADDR_BKP_BOOTVER    	( RTC_BKP_DR1 )
#define ADDR_BKP_APPVER			( RTC_BKP_DR2 )


int8_t boot_flag_write(uint16_t value);
int8_t boot_version_write(uint16_t value);
uint16_t boot_flag_read(void);
void boot_check(void);



#ifdef __cplusplus
}
#endif

#endif 

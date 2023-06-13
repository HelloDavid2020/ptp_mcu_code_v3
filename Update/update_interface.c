#include "update_interface.h"


static __IO process_state_t update_in_process = UPDATE_RELEASE;

const char *PROCESS_BUSY[]={
		"UPDATE PROCESS RELEASE",
		"UPDATE PROCESS CAN",
		"UPDATE PROCESS UART",
};

process_state_t update_process_busy(void)
{
	return update_in_process;
}

void update_process_release(void)
{
	update_in_process = UPDATE_RELEASE;
	printf("NOTE: UPATE PROCSS IS RELEASE !\r\n");

}

void update_process_take(process_state_t state)
{
	update_in_process = state;
	printf("UPDATE TAKE TOKEN: %s !\r\n",PROCESS_BUSY[state]);
}

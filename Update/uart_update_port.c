#include "uart_update_port.h"

ringbuffer_t iap_recv_buf;


uint8_t rx_buf[IAP_BUFFER_SIZE*2+256];

static callback_rx_t callback_iap_rx;

void iap_rx_callback(uint8_t data)
{
    lib_ringbuf_write_byte(&iap_recv_buf, data);
}


void iap_port_register(callback_rx_t callback_function)
{
	callback_iap_rx = callback_function;
}


void iap_port_init(void)
{
	// command length + dta length
    lib_ringbuf_create(&iap_recv_buf, IAP_BUFFER_SIZE*2+256);
    iap_port_register(iap_rx_callback);

    iap_rx_enable();

}




void iap_rx_enable(void)
{
    LL_USART_EnableIT_RXNE(USART1); // Enable RX Interrupt
    LL_USART_EnableIT_IDLE(USART1);
    LL_USART_ClearFlag_IDLE(USART1);
}

void iap_rx_disable(void)
{
    LL_USART_DisableIT_RXNE(USART1); // Enable RX Interrupt
    LL_USART_DisableIT_IDLE(USART1);
    LL_USART_ClearFlag_IDLE(USART1);
}



void iap_send(const char *pData)
{
	printf(pData);
}



void iap_rx_handler(void)
{
	uint8_t data = 0;

	if(LL_USART_IsActiveFlag_ORE(USART1))
	{
		printf("*** ORE ***\r\n");
		LL_USART_ClearFlag_ORE(USART1);
		data=LL_USART_ReceiveData8(USART1);
	}
	if(LL_USART_IsActiveFlag_IDLE(USART1))
	{
		LL_USART_ClearFlag_IDLE(USART1);
		at_cmd_recv_handle(&iap_recv_buf);
	}
	if(LL_USART_IsActiveFlag_RXNE(USART1))
	{
		data =LL_USART_ReceiveData8(USART1);
		callback_iap_rx(data);
	}

}




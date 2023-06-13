#ifndef _IAP_PORT_H_
#define _IAP_PORT_H_

#include <update_interface.h>




#ifdef __cplusplus
extern "C" {
#endif


typedef void (* callback_rx_t)(uint8_t);
void rx_callback(USART_TypeDef *USARTx, ringbuffer_t *ring_buffer);

extern ringbuffer_t iap_recv_buf;


extern void iap_rx_enable(void);
extern void iap_rx_disable(void);
extern void iap_send(const char *pData);
extern void iap_port_init(void);
extern void iap_rx_handler(void);



#ifdef __cplusplus
}
#endif

#endif 

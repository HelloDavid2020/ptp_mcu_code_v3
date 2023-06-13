#include "test.h"
__IO uint8_t blink_flag = 0;

void tx_control_test(void)
{
    if(blink_flag)
    {
        if((uint32_t)SENSOR_SINGLE_LEVEL == 0)
        {
            blink_flag = 0;
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);   
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);          

            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);   
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);
            
            // 3ms
            bsp_uart_send_byte(USART2,0x55);
            bsp_uart_send_byte(USART2,0x66);
            bsp_uart_send_byte(USART2,0x55);        


            
        }
    }
}

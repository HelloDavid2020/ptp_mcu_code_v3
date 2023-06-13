#include "lcd.h"

static void lcd_send(const char * str);
static char avr_str[30];
static char max_str[30];
static char min_str[30];
static char pulse_str[30];

capture_data_t capture_data;

void lcd_infor_update(capture_data_t* data, uint32_t time_interval)
{
    static uint32_t update_flag = 0;
    static uint32_t time_start =0;

    if(( HAL_GetTick() - time_start) >= time_interval/3)
    {
        time_start = HAL_GetTick();
        switch(update_flag)
        {
            case 0:
                update_flag =1;
                sprintf(avr_str,"x0.val=%d\xFF\xFF\xFF", data->avr_value);        
                lcd_send(avr_str);
            break;

            case 1:
                update_flag =2;
                sprintf(max_str,"x1.val=%d\xFF\xFF\xFF", data->max_value);      
                lcd_send(max_str);
                break;
            
            case 2:
                update_flag =3;
                sprintf(min_str,"x2.val=%d\xFF\xFF\xFF", data->min_value);
                lcd_send(min_str);
                break;
            
            
            case 3:
                update_flag =0;
                sprintf(pulse_str,"x3.val=%d\xFF\xFF\xFF", pulse_filter);
                lcd_send(pulse_str);
                break;
            
            default:
                update_flag =0;
                break;

        
        }    
    
    }
  
    
}





void lcd_send(const char * str)
{
     bsp_uart_send_string(USART3, str);
}


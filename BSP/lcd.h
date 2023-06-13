#ifndef LCD_H_
#define LCD_H_

#include "main.h"

typedef struct
{
    uint32_t real_value;
    uint32_t avr_value;
    uint32_t max_value;
    uint32_t min_value;
    uint32_t sum_value;
    uint32_t sample_cnt;

}capture_data_t;
    

extern capture_data_t capture_data;
void lcd_infor_update(capture_data_t* data, uint32_t time_interval);
#endif /* BOARD_H_ */

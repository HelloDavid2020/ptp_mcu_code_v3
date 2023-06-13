#include "event.h"
#include "tim.h"
#include "board.h"
#include "lcd.h"


static event_t system_event = EVT_NULL;

void system_event_send(event_t evt)
{
    system_event |= evt;
}

void system_event_get(event_t * evt)
{
    * evt = system_event;
}

void system_event_clear(event_t evt)
{
    system_event &= ~(evt);
}

void system_event_clear_all(void)
{
    system_event= EVT_NULL;
}





void system_event_handler(void)
{
    event_t evt;

    system_event_get(&evt);


    if(evt == EVT_NULL)
        return;


    if(evt & EVT_RESET)
    {
        system_event_clear(EVT_RESET);
        system_reset();
        printf("EVT: EVT_RESET\r\n");
    }
    if(evt & EVT_START)
    {
        system_event_clear(EVT_START);
        printf("EVT: EVT_START\r\n");
        
        memset(&capture_data, 0x00, sizeof (capture_data_t));
        capture_data.min_value = 999999;
        
    }
    if(evt & EVT_LIGHTON)
    {
        system_event_clear(EVT_LIGHTOFF);
        LIGHT_ON();
        printf("EVT: EVT_LIGHTON\r\n\r\n");
    }
   
    if(evt & EVT_LIGHTOFF)
    {
        system_event_clear(EVT_LIGHTOFF);
        LIGHT_OFF();

        printf("EVT: EVT_LIGHTOFF\r\n\r\n");
    }

    if(evt & EVT_LIGHTTEST)
    {
        system_event_clear(EVT_LIGHTTEST);
        printf("EVT: EVT_LIGHTTEST\r\n");
         capture_start(&input_capture);
        
        
    }

    system_event_clear_all();
    

}

#ifndef EVENT_H_
#define EVENT_H_

#include "main.h"



typedef enum
{
    EVT_RESET       = (1<<0),
    EVT_LIGHTON     = (1<<1),
    EVT_LIGHTOFF    = (1<<2),
    EVT_LIGHTTEST   = (1<<3),
    EVT_START       = (1<<4),

    EVT_NULL            = (1<<30),
} event_t;


void system_event_send(event_t evt);

void system_event_get(event_t * evt);
void system_event_clear(event_t evt);
void system_event_clear_all(void);

void system_event_handler(void);



#endif 



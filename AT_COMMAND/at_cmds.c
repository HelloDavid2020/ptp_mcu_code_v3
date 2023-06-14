#include "at_cmds.h"
#include "board.h"
#include "main.h"
#include "event.h"


static void at_cmd_parse(uint8_t *cmd );
static void at_iap_start( char* cmd ) ;
static void at_iap_data( char* cmd );
static void at_iap_end( char* cmd );
static void at_iap_abort( char* cmd );
static void at_ok( char* cmd );
static void at_erase( char* cmd );
static void at_read( char* cmd );
static void at_write( char* cmd );
static void at_reset( char* cmd );
static void at_info( char* cmd );
static void at_boot( char* cmd );
static void at_sample( char* cmd );
static void at_light_on( char* cmd );
static void at_light_off( char* cmd );
static void at_light_test( char* cmd );
static void at_status( char* cmd );
static void at_filter( char* cmd );
static void at_start( char* cmd );
static void at_pulse_capture_filter( char* cmd );
static void at_led_on_time( char* cmd );

                       

const at_cmd_items_t at_command_items[] =
{
    // UPGRADE

    {"AT+IAPSTART",         at_iap_start},
    {"AT+IAPDATA",          at_iap_data},
    {"AT+IAPEND",           at_iap_end},
    {"AT+IAPABORT",         at_iap_abort},
    {"AT+OK",               at_ok},
    {"AT+ERASE",            at_erase},
    {"AT+READ",             at_read},
    {"AT+WRITE",            at_write},
    {"AT+RESET",            at_reset},
    {"AT+INFO",             at_info},
    {"AT+BOOT",             at_boot},
    {"AT+SAMPLE",           at_sample},
    {"AT+LIGHTON",          at_light_on},
    {"AT+LIGHTOFF",         at_light_off},
    {"AT+LIGHTTEST",        at_light_test},

    {"AT+STATUS",           at_status},
    {"AT+FILTER",           at_filter},
    {"AT+START",            at_start},
    {"AT+PULSEFILTER",      at_pulse_capture_filter}, // PULSE_CAPTURE_FILTERCNT
    {"AT+LEDONTIME",        at_led_on_time}, // PULSE_CAPTURE_FILTERCNT


};


static void at_led_on_time( char* cmd )
{
    uint32_t ontime = 0;
    char *p = strstr(cmd,"AT+LEDONTIME=");
    if(p)
    {
        char *cmd_target =strstr(cmd,"AT+LEDONTIME=");

        if(sscanf((char*)cmd_target,"AT+LEDONTIME=%d", &ontime) == 1)
        {
						if(led_on_time<=10 && led_on_time >= 1)
            {
							led_on_time = ontime;
							printf("+LEDONTIME=%d\r\n",led_on_time);
						}
						else
						{
							printf("ERROR: Invalid Data.\r\n");
							printf("+LEDONTIME=%d\r\n",led_on_time);
	
						}
        }

       
    }
}


static void at_pulse_capture_filter( char* cmd )
{
    uint32_t pulse_filter_cnt = 0;
    char *p = strstr(cmd,"AT+PULSEFILTER=");
    if(p)
    {
        char *cmd_target =strstr(cmd,"AT+PULSEFILTER=");

        if(sscanf((char*)cmd_target,"AT+PULSEFILTER=%d", &pulse_filter_cnt) == 1)
        {
        
            pulse_filter = pulse_filter_cnt;
            printf("+PULSEFILTER=%d\r\n",pulse_filter);
        }

       
    }
}

static void at_start( char* cmd )
{
   system_event_send(EVT_START);

}

static void at_light_on( char* cmd )
{
   system_event_send(EVT_LIGHTON);

}

static void at_light_off( char* cmd )
{
   system_event_send(EVT_LIGHTOFF);
}

static void at_light_test( char* cmd )
{
   system_event_send(EVT_LIGHTTEST);

}




static void at_status( char* cmd )
{
    if( system_sleep == 1)
        printf("+STATUS=SLEEP\r\n");
    else
        printf("+STATUS=RUN\r\n");
}



static void at_sample( char* cmd )
{
    uint32_t sample = 0;
    char *p = strstr(cmd,"AT+SAMPLE=");
    if(p)
    {
        char *cmd_target =strstr(cmd,"AT+SAMPLE=");

        if(sscanf((char*)cmd_target,"AT+SAMPLE=%d", &sample) == 1)
        {
        
            sample_count =    sample;
            printf("+SAMPLE=%d\r\n",sample_count);
        }

       
    }

}

  //    S2  S3  filter
  //    L   L   RED
  //    H   H   GREEN
  //    L   H   BLUE
  //    H   L   WHITE


static void at_filter( char* cmd )
{
    char *p = strstr(cmd,"AT+FILTER=RED");
    if(strstr(cmd,"AT+FILTER=RED"))
    {
        SENSOR_S2_LOW(); 
        SENSOR_S3_LOW(); 
        printf ("FLITER=RED, S2=0,S3=0\r\n");
    }
    else if(strstr(cmd,"AT+FILTER=GREEN"))
    {
        SENSOR_S2_HIGH(); 
        SENSOR_S3_HIGH(); 
        printf ("FLITER=GREEN, S2=1,S3=1\r\n");

    }        
    else if(strstr(cmd,"AT+FILTER=BLUE"))
    {
        SENSOR_S2_LOW(); 
        SENSOR_S3_HIGH(); 
        printf ("FLITER=BLUE, S2=0,S3=1\r\n");

    }    
    else if(strstr(cmd,"AT+FILTER=WHITE"))
    {
        SENSOR_S2_HIGH(); 
        SENSOR_S3_LOW(); 
        printf ("FLITER=WHITE, S2=1,S3=0\r\n");

    }      
    
}



static void at_ok( char* cmd )
{
    printf("OK\r\n");
}

static void at_iap_start( char* cmd )
{
    char *p = strstr(cmd,"=");  
  
}


static void at_iap_data( char* cmd )
{

}


static void at_iap_end( char* cmd )
{
//    iap_handle.process_state = IAP_END;
}



static void at_iap_abort( char* cmd )
{
//    iap_handle.process_state = IAP_ABORT;
}


static void at_erase( char* cmd )
{


}


static void at_read( char* cmd )
{
 

}
static void at_reset( char* cmd )
{
    printf("-> System Reset...\r\n");
    system_reset();

}

static void at_info( char* cmd )
{

    printf("===================================\r\n");
    printf("-> PTP BOX\r\n");
    printf("-> Version= 3.0.0\r\n");
    printf("-> BuildTime= %s\r\n",build_time());

}


static void at_boot( char* cmd )
{
    printf("-> Jump to Bootloader\r\n");

}


static void at_write( char* cmd )
{


}



static void at_cmd_parse(uint8_t *cmd )
{
    int32_t total_items = sizeof( at_command_items ) / sizeof( at_cmd_items_t );
    char *  ptr;

    if(cmd == NULL) 
        return;

    for( int32_t i = 0; i<total_items; i++ )
    {
        if(strlen((char*)cmd)>2)
        {
            ptr = strstr((char*)cmd, at_command_items[i].p_at_command);    
            if( ptr )
            {
                at_command_items[i].pAtFunc( (char*)cmd );
                return;
            }
        }
    }
//    DBG_TRACE("UNKOWN COMMAND\r\nERROR\r\n");
}


static at_cmd_buffer_t at_commands;

void at_cmd_recv_handle( ringbuffer_t * input_cmd )
{

    while( lib_ringbuf_read_byte(input_cmd, &at_commands.rx_buff[at_commands.rx_index]))
    {
        at_commands.rx_index ++;

        if(at_commands.rx_index >2)
        {
            if( at_commands.rx_buff[at_commands.rx_index-2] == 0x0d && at_commands.rx_buff[at_commands.rx_index-1] == 0x0a  )
            {
               
                at_commands.rx_buff[at_commands.rx_index - 2] = '\0';   
                
                    at_cmd_parse( at_commands.rx_buff);

                memset(at_commands.rx_buff, 0x00, sizeof at_commands.rx_buff);
                at_commands.rx_index = 0;
            }
        }


        if( at_commands.rx_index >= CMD_LENGTH ) at_commands.rx_index =0;
    }
}



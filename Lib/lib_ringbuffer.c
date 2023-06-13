#include "lib_ringbuffer.h"


int16_t lib_ringbuf_create(ringbuffer_t *rb,int size)
{  
    rb->size  = size + 1;/* include empty elem */
    rb->ReadPointer =0;
    rb->WritePointer   =0;
    rb->elems =(uint8_t *)malloc(rb->size);
    if(rb->elems == NULL)
        return -1;
    return 0;
}

void lib_ringbuf_freee(ringbuffer_t *rb)
{  
   if(rb)
    free(rb->elems);
}

void lib_ringbuf_clear(ringbuffer_t *rb)
{  
    rb->ReadPointer =0;
    rb->WritePointer=0;
}

int16_t lib_ringbuf_is_full(ringbuffer_t *rb)
{  
    return(rb->WritePointer +1)% rb->size == rb->ReadPointer;
}

int16_t lib_ringbuf_is_empty(ringbuffer_t *rb)
{  
    return rb->WritePointer == rb->ReadPointer;
}

int16_t lib_ringbuf_get_free(ringbuffer_t *rb)
{ 
    if(lib_ringbuf_is_empty(rb))
    {
        return rb->size - 1;
    }
    
    if(rb->WritePointer < rb->ReadPointer){
        return rb->ReadPointer - rb->WritePointer - 1;
    }
    
    return (rb->size - rb->WritePointer) + rb->ReadPointer - 1;
}

int16_t lib_ringbuf_write_byte(ringbuffer_t *rb, uint8_t elem)
{  
    if((rb->WritePointer +1)% rb->size != rb->ReadPointer)
    {
       rb->elems[rb->WritePointer]=elem;
       rb->WritePointer =(rb->WritePointer +1)% rb->size;
       return 1;
    }
    else
    {
      return 0;
    }
}

int16_t lib_ringbuf_read_byte(ringbuffer_t *rb, uint8_t *elem)
{  
    if(rb->WritePointer != rb->ReadPointer)
    {
       *elem = rb->elems[rb->ReadPointer];
       rb->ReadPointer =(rb->ReadPointer +1)% rb->size;
       return 1;
    }
    else
    {
      return 0;
    }
}


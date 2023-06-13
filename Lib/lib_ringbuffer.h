#ifndef __LIB_RINGBUFFER_H__
#define __LIB_RINGBUFFER_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct 
{  
    uint16_t         size;
    uint16_t         ReadPointer;
    uint16_t         WritePointer;
    uint8_t          *elems;
} ringbuffer_t;


extern int16_t lib_ringbuf_create(ringbuffer_t *rb,int size);
extern void lib_ringbuf_freee(ringbuffer_t *rb);
extern void lib_ringbuf_clear(ringbuffer_t *rb);
extern int16_t lib_ringbuf_is_full(ringbuffer_t *rb);
extern int16_t lib_ringbuf_is_empty(ringbuffer_t *rb);
extern int16_t lib_ringbuf_get_free(ringbuffer_t *rb);
extern int16_t lib_ringbuf_write_byte(ringbuffer_t *rb, uint8_t elem);
extern int16_t lib_ringbuf_read_byte(ringbuffer_t *rb, uint8_t *elem);


#endif


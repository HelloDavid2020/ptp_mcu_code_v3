#ifndef _LIB_COMMFUNC_H_
#define _LIB_COMMFUNC_H_

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {FAILED = 0, PASSED = !FAILED} test_status_t;
extern int lib_is_digitstr(char *str);
extern short lib_bin_search_u16(const unsigned short array[], unsigned short size, unsigned short target);
extern short lib_bin_search_u8(const unsigned char array[], unsigned short size, unsigned char target);
extern char * lib_n_strstr(const char *src, int len, const char *dst);
extern void lib_set_bit(unsigned char *data,unsigned char bit_num);
extern void lib_clear_bit(unsigned char *data,unsigned char bit_num);
extern int lib_hex2ascii(const void * hex, char * ascii_str, unsigned int hex_len);
extern int lib_ascii2hex(const char *inputdata, unsigned char *outbuffer, unsigned int outbuffer_len);
extern void lib_endian_convert(void* dest, void* src, unsigned char len);
extern void lib_insert_sort( int16_t *data, uint16_t len );

extern test_status_t lib_buffer_compare(const uint8_t* pBuffer, uint8_t* pBuffer1, int16_t BufferLength);
extern uint8_t lib_is_valid_ip(const char *ip);
extern  void lib_string_squeeze(char *str, char ch)  ;
extern int lib_delete_substring(const char *str, const char *sub_str, char *result_str);

#endif


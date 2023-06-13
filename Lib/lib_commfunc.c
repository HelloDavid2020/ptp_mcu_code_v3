#include "lib_commfunc.h"


int lib_is_digitstr(char *str)

{
    return (strspn(str, "0123456789")==strlen(str));
}

uint8_t lib_is_valid_ip(const char *ip)
{
   int dots = 0; 
      int setions = 0; 
 
     if (NULL == ip || *ip == '.') { 
         return 0;
     }   
 
     while (*ip) {
 
         if (*ip == '.') {
             dots ++; 
             if (setions >= 0 && setions <= 255) { 
                 setions = 0;
                 ip++;
                 continue;
             }   
             return 0;
         }   
         else if (*ip >= '0' && *ip <= '9') {
             setions = setions * 10 + (*ip - '0');
         } else 
             return 0;
         ip++;   
     }   
     if (setions >= 0 && setions <= 255) {
         if (dots == 3) {
             return 1;
         }   
     }   
 
     return 0;
}


short lib_bin_search_u16(const unsigned short array[], unsigned short size, unsigned short target)
{
    short start = 0;
    short end = size - 1;
    short mid;

    while ((end - start) > 1)
    {
        mid = (int16_t)((start + end) / 2);
        if (target < array[mid])
        {
            start = mid;
        }
        else if (target > array[mid])
        {
            end = mid;
        }
        else
        {
            return mid;
        }
    }

    return start;
}


short lib_bin_search_u8(const unsigned char array[], unsigned short size, unsigned char target)
{
    short start = 0;
    short end = size - 1;
    short mid;

    while ((end - start) > 1)
    {
        mid = (int16_t)((start + end) / 2);
        if (target < array[mid])
        {
            start = mid;
        }
        else if (target > array[mid])
        {
            end = mid;
        }
        else
        {
            return mid;
        }
    }

    return start;
}


char * lib_n_strstr(const char *src, int len, const char *dst) {
    char *ptr = (char *)src;
    int dst_len = strlen(dst);
    len -= dst_len;
    len++;
    if(len < 0)
    {
        return NULL;
    }
    while(len--)
    {
        if(strncmp(&ptr[len], dst, dst_len) == 0)
        {

            return &ptr[len];
        }
    }
    return NULL;
}


void lib_set_bit(unsigned char *data,unsigned char bit_num)
{
    (*data)|=(1<<bit_num);
}

void lib_clear_bit(unsigned char *data,unsigned char bit_num)
{
    (*data)&=~(1<<bit_num);
}


int lib_hex2ascii(const void * hex, char * ascii_str, unsigned int hex_len)
{
    // const char * ascTable = {"0123456789abcdef"};
    const char * ascTable = {"0123456789ABCDEF"};

    int pos = 0;

    for(int i = 0; i < hex_len; i++)
    {
        ascii_str[pos++] = ascTable[*((unsigned char *)hex + i) >> 4];
        ascii_str[pos++] = ascTable[*((unsigned char *)hex + i) & 0x0F];
    }

    ascii_str[pos] = '\0'; 

    return pos;
}


int lib_ascii2hex(const char *inputdata, unsigned char *outbuffer, unsigned int outbuffer_len)
{
    unsigned char *tmp_p = outbuffer;

    int i,j, pos;
    pos = 0;
    char inputchar;

    for(i = 0; i < outbuffer_len; i++)
    {
        inputchar=inputdata[i];
        inputchar=(char)toupper((int)inputchar);
        j = ('A' > inputchar)?(inputchar-'0'):(inputchar-'A'+10);

        if(0 == i%2)
        {
          tmp_p[pos] = j<<4;
        }
        else
        {
          tmp_p[pos++] += j;
        }
    }

    return pos;
}


void lib_endian_convert(void* dest, void* src, unsigned char len)
{
    unsigned short i;
    for(i=0;i<len;i++)
    {
        *(unsigned char *)((unsigned char *)dest+i)=*(unsigned char *)((unsigned char *)src+len-1-i);
    }
}

void lib_insert_sort( int16_t *data, uint16_t len )  
{  
    int i, j, temp;  

    for(i = 1; i < len; i ++)  
    {  
        temp = data[i];  
        for(j = i - 1; j >= 0; j --)  
        {  
            if(data[j] > temp)  
            {  
                data[j + 1] = data[j];      
            }else  
            {  
                break;  
            }  
        }  
        data[j + 1] = temp;  
    }  
}


test_status_t lib_buffer_compare(const uint8_t* pBuffer, uint8_t* pBuffer1, int16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }
  return PASSED;
}


 
void lib_string_squeeze(char *str, char ch)  
{  
    if(str == NULL)
        return;
    int i,j;  
    for (i = 0, j = 0; str[i] != '\0'; i++)  
    {  
        if (str[i] != ch)  
        {  
            str[j++] = str[i];  
        }  
    }  
    str[j] = '\0'; 
} 


int lib_delete_substring(const char *str, const char *sub_str, char *result_str)  
{  
    int count = 0;  
    int str_len = strlen(str);  
    int sub_str_len = strlen(sub_str);  
  
    if (str == NULL)  
    {  
        result_str = NULL;  
        return 0;  
    }  
  
    if (str_len < sub_str_len || sub_str == NULL)  
    {  
        while (*str != '\0')  
        {  
            *result_str = *str;  
            str++;  
            result_str++;  
        }  
  
        return 0;  
    }  
  
    while (*str != '\0')  
    {  
        while (*str != *sub_str && *str != '\0')  
        {  
            *result_str = *str;  
            str++;  
            result_str++;  
        }  
  
        if (strncmp(str, sub_str, sub_str_len) != 0)  
        {  
            *result_str = *str;  
            str++;  
            result_str++;  
            continue;  
        }  
        else  
        {  
            count++;  
            str += sub_str_len;  
        }  
    }  
  
    *result_str = '\0';  
  
    return count;  
} 


#include "stdint.h"

void* memset(void* str, int c, size_t n)
{
    uint8_t* temp = (uint8_t*)str;
    for(size_t i=0; i < n; i++)
        temp[i] = c;
    
    return (void*)temp;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    uint8_t* temp_dest = (uint8_t*)dest;
    uint8_t* temp_src = (uint8_t*)src;
    
    for(size_t i=0; i < n; i++)
        temp_dest[i] = temp_src[i];
    
    return (void*)temp_dest;
}

char* strcpy(char* dest, const char* src)
{
    char* temp = dest;
    while((*temp++ = *src++));
    return dest;
}

size_t strlen(const char* str)
{
    size_t count = 0;
    char* temp = (char*)str;
    
    while(*temp++)
        count++;
    
    return count;
}

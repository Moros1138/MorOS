#ifndef STRING_H
#define STRING_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memset(void* str, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t count);

char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t count);

#ifdef __cplusplus
}
#endif


#endif
#ifndef STDLIB_H
#define STDLIB_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib/printf.h"

// MEMORY MANAGEMENT
void* malloc(size_t count);
void* realloc(void* ptr, size_t new_size);
void* calloc(size_t num, size_t size);

void  free(void* ptr);

// RANDOM NUMBERS
void srand(uint32_t seed);
int rand();

#define RAND_MAX 2147483647


#ifdef __cplusplus
}
#endif

#endif // STDLIB_H

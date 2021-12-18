#ifndef STDINT_H
#define STDINT_H

#ifdef _cplusplus
export "C" {
#endif

// 8 bit
typedef char                int8_t;
typedef unsigned char       uint8_t;

// 16 bit
typedef short               int16_t;
typedef unsigned short      uint16_t;

// 32 bit
typedef int                 int32_t;
typedef unsigned int        uint32_t;

// 64 bit
typedef long long           int64_t;
typedef unsigned long long  uint64_t;

// maximum signed integer type
typedef long long           intmax_t;
// maximum unsigned integer type
typedef unsigned long long  uintmax_t;

// unsigned integer type that can represent any possible size of any object
typedef unsigned long   size_t;

// signed integer type capable of holding a pointer 
typedef long            intptr_t;
// unsigned integer type capable of holding a pointer
typedef unsigned long   uintptr_t;

// false/true
#ifndef __cplusplus
typedef unsigned char   bool;
enum { false, true };
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define UNUSED(x) (void)(x)

#ifdef _cplusplus
}
#endif

#endif // STDINT_H


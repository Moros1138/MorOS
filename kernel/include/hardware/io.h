#ifndef IO_H
#define IO_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// read byte from specified port
uint8_t inb(uint16_t port);

// write byte at specified port
void outb(uint16_t port, uint8_t val);

// read word from specified port
uint16_t inw(uint16_t port);

// write word at specified port
void outw(uint16_t port, uint16_t val);

#ifdef __cplusplus
}
#endif

#endif

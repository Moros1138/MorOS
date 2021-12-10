#ifndef SERIAL_H
#define SERIAL_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// initializer serial port COM1
bool serial_init();
// read byte from serial port COM1
uint8_t serial_read();
// write byte to serial port COM1
void serial_write(uint8_t a);
// write string to serial port COM1
void serial_puts(const char* str);

bool serial_running();

#ifdef __cplusplus
}
#endif

#endif

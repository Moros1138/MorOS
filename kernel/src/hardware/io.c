#include "hardware/io.h"

uint8_t inb(uint16_t port)
{
    uint8_t r;
    __asm__ __volatile__("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (val));
}

uint16_t inw(uint16_t port)
{
    uint16_t r;
    __asm__ __volatile__("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

void outw(uint16_t port, uint16_t val)
{
    __asm__ __volatile__("outw %1, %0" : : "dN" (port), "a" (val));
}

#include "io.h"

namespace MorOS
{
    // write a byte out to the specified port.
    void outb(uint16_t port, uint8_t value)
    {
        asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
    }
    
    void outw(uint16_t port, uint16_t value)
    {
        asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
    }

    // read a byte from the specified port.
    uint8_t inb(uint16_t port)
    {
        uint8_t ret;
        asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }
    
    // read a word (16-bits) from the specified port.
    uint16_t inw(uint16_t port)
    {
        uint16_t ret;
        asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }

} // MorOS

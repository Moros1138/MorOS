#ifndef _MOROS_IO_H
#define _MOROS_IO_H

#include "types.h"

namespace MorOS
{
    // write 8-bit value to specified port
    void outb(uint16_t port, uint8_t value);
    // write 16-bit value to specified port
    void outw(uint16_t port, uint16_t value);
    // read 8-bit value from specified port
    uint8_t inb(uint16_t port);
    // read 16-bit value from specified port
    uint16_t inw(uint16_t port);

} // MorOS

#endif // _MOROS_IO_H

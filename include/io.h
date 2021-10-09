#ifndef _MOROS_IO_H
#define _MOROS_IO_H

#include "types.h"

namespace MorOS
{

    void outb(uint16_t port, uint8_t value);
    uint8_t inb(uint16_t port);
    uint16_t inw(uint16_t port);

} // MorOS

#endif // _MOROS_IO_H

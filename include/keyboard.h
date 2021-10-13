#ifndef _MOROS_KEYBOARD_H
#define _MOROS_KEYBOARD_H

#include "types.h"
#include "isr.h"
#include "io.h"
#include "monitor.h"

namespace MorOS
{
    class Keyboard
    {
    public:
        Keyboard();
        static void handler(registers_t regs);
        
        static uint16_t last_key;
    };
} // MorOS

#endif // _MOROS_KEYBOARD_H


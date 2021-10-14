#ifndef _MOROS_MOUSE_H
#define _MOROS_MOUSE_H

#include "types.h"
#include "isr.h"
#include "io.h"

#include "hardware/monitor.h"

namespace MorOS
{
    class Mouse
    {
    public:
        Mouse();
        
        static Mouse* activeMouse;

        static void handler(registers_t regs);
        
        static int32_t cursor_x;
        static int32_t cursor_y;
        static uint8_t buttons;
        static uint8_t buffer[3];
        static uint8_t offset;
    };
} // MorOS

#endif // _MOROS_MOUSE_H

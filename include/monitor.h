#ifndef _MOROS_MONITOR_H
#define _MOROS_MONITOR_H

#include "types.h"
#include "io.h"

namespace MorOS
{
    constexpr size_t VGA_WIDTH  = 80;
    constexpr size_t VGA_HEIGHT = 25;

    constexpr uint8_t VGA_SET_CURSOR_HIGH_BYTE  = 14;
    constexpr uint8_t VGA_SET_CURSOR_LOW_BYTE   = 15;

    constexpr uint16_t VGA_COMMAND_PORT         = 0x03d4;
    constexpr uint16_t VGA_DATA_PORT            = 0x03d5;

    class Monitor
    {
    public:
        Monitor();
        ~Monitor();

        static Monitor* activeMonitor;
    
    public:
        void putc(char ch);
        void puts(char* str);

    private:
        void move_cursor();
        void scroll();

    private:
        // video ram buffer
        uint16_t*   buffer;
        
        // cursor and position tracking
        uint16_t    cursor_x;
        uint16_t    cursor_y;
        size_t      index;

        // current attribute
        uint8_t     attribute;
    };


} // MorOS

#endif // _MOROS_MONITOR_H

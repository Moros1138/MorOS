#ifndef _MOROS_MONITOR_H
#define _MOROS_MONITOR_H

#include <stdarg.h>

#include "types.h"
#include "io.h"
#include "memory.h"

namespace MorOS
{
    constexpr size_t VGA_WIDTH  = 80;
    constexpr size_t VGA_HEIGHT = 25;
    
    constexpr uint8_t VGA_SET_CRSR_MAX_SCAN   = 0x09;
    constexpr uint8_t VGA_SET_CRSR_START      = 0x0a;
    constexpr uint8_t VGA_SET_CRSR_END        = 0x0b;
    constexpr uint8_t VGA_SET_CRSR_HIGH_BYTE  = 0x0e;
    constexpr uint8_t VGA_SET_CRSR_LOW_BYTE   = 0x0f;

    constexpr uint16_t VGA_COMM_PORT          = 0x03d4;
    constexpr uint16_t VGA_DATA_PORT          = 0x03d5;

    class Monitor
    {
    public:
        Monitor();
        ~Monitor();

        static Monitor* activeMonitor;
    
    public:
        void putc(char ch);
        void puts(char* str);
        void putdec(uint32_t num, bool bSigned = false);
        void puthex(uint32_t num);

        void clear();
        void show_cursor(bool state);
        void set_color(uint8_t fg, uint8_t bg);

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

    void printf(char* fmt, ...);

} // MorOS

#endif // _MOROS_MONITOR_H

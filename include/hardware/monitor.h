#ifndef _MOROS_MONITOR_H
#define _MOROS_MONITOR_H

#include <stdarg.h>

#include "types.h"
#include "io.h"
#include "memory.h"

namespace MorOS
{

    class Monitor
    {
    public:
        Monitor();
        ~Monitor();

        static Monitor* activeMonitor;
    
    enum class Mode
    {
        Text,
        Graphics
    };

    public: // MODE SWITCHING
        void SetMode(Monitor::Mode m);
        void Swap();
        
    public: // STUFF
        int32_t Width();
        int32_t Height();
    
    public: // TEXT MODE FUNCTIONS        
        void putc(char ch);
        void puts(char* str);
        void putdec(uint32_t num, bool bSigned = false);
        void puthex(uint32_t num);
        
        void clear();
        void show_cursor(bool state);
        void set_color(uint8_t fg, uint8_t bg);
    
    public: // GRAPHICS MODE FUNCTIONS
        void switchTo13h();

    private:
        void move_cursor();
        void scroll();
        
    private: // Text Mode Internal Tracking
        
        // video ram buffer
        uint16_t    pTextBuffer[2000];

        // cursor and position tracking
        uint16_t    cursor_x;
        uint16_t    cursor_y;
        size_t      index;

        // current attribute
        uint8_t     attribute;

    private: // Graphics Mode Internal Tracking
        uint8_t     pGraphicsBufffer[64000];
        
    private: // Common Internal Tracking
        // current screen size
        int32_t     width;
        int32_t     height;

        Monitor::Mode mode;
    };

    void printf(char* fmt, ...);

} // MorOS

#endif // _MOROS_MONITOR_H

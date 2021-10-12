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
    
    public:
        void putc(char ch);
        void puts(char* str);
        void putdec(uint32_t num, bool bSigned = false);
        void puthex(uint32_t num);

    private:
    };

    void printf(char* fmt, ...);

} // MorOS

#endif // _MOROS_MONITOR_H

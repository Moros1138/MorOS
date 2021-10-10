#ifndef _MOROS_TIMER_H
#define _MOROS_TIMER_H

#include "monitor.h"
#include "io.h"
#include "isr.h"

namespace MorOS
{
    class Timer
    {
    public:
        Timer();
        Timer(uint32_t freq);
        ~Timer();
        
        void init(uint32_t freq);
        void set(uint32_t freq);
        
        static void callback(registers_t regs);
        static uint32_t tick;
    }; // Timer

} // MorOS

#endif // _MOROS_TIMER_H
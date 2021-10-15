#include "hardware/timer.h"

namespace MorOS
{
    // define it so we can use it in the callback!
    uint32_t Timer::tick = 0;

    Timer::Timer()
    {
        tick = 0;
        register_interrupt_handler(IRQ0, &MorOS::Timer::callback);
    }

    Timer::Timer(uint32_t freq)
    {
        tick = 0;
        register_interrupt_handler(IRQ0, &MorOS::Timer::callback);
        set(freq);
    }
    
    Timer::~Timer()
    { }
    
    void Timer::set(uint32_t freq)
    {
        // The value we send to the PIT is the value to divide it's input clock
        // PIT input clock: 1193180 Hz
        
        // divide the PIT clock speed by the frequency we want
        // divisor must be small enough to fit into 16-bits.
        // the higher the divisor the faster the interrupt
        uint32_t divisor = 1193180 / freq;
        
        if(divisor >= 0x10000)
            divisor = 0xffff;
        
        // Send the command byte.
        outb(0x43, 0x36);

        // Send the frequency divisor.
        outb(0x40, (uint8_t)(divisor & 0xFF));
        outb(0x40, (uint8_t)((divisor>>8) & 0xFF));
    }

    void Timer::callback(registers_t regs)
    {
        tick++;
    }

} // MorOS
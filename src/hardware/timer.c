#include "hardware/timer.h"
#include "hardware/io.h"
#include "hardware/interrupts.h"

static uint32_t timer_tick = 0;

void timer_callback(registers_t regs)
{
    UNUSED(regs);
    timer_tick++;
}

void timer_init()
{
    register_interrupt_handler(IRQ0, &timer_callback);
    
    outb(0x43, 0x36);
    uint32_t divisor = 1193180 / 1193;

    // divisor low byte
    outb(0x40, (divisor & 0x00ff) >> 0);
    // divisor high byte
    outb(0x40, (divisor & 0xff00) >> 8);

    // divisor = 1193180 / frequency;
    // because we want a divisor of about 1000, frequency = 1193
}

uint32_t timer_get_time_point()
{
    return timer_tick;
}
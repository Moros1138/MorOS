#ifndef _MOROS_IDT_H
#define _MOROS_IDT_H

#include "types.h"
#include "memory.h"

// Lets us access our ASM functions from our C code.

extern "C" void idt_flush(uint32_t);

namespace MorOS
{
    class InterruptManager
    {
        class GateDescriptor
        {
        private:
            uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
            uint16_t sel;                 // Kernel segment selector.
            uint8_t  always0;             // This must always be zero.
            uint8_t  flag_byte;               // More flags. See documentation.
            uint16_t base_hi;             // The upper 16 bits of the address to jump to.
        public:
            GateDescriptor();
            GateDescriptor(uint32_t base, uint16_t sel, uint8_t flags);
            void Set(uint32_t base, uint16_t sel, uint8_t flags);
        }__attribute__((packed));

    private:
        GateDescriptor  interrupts[256];

    public:
        InterruptManager();
    };
    

} // MorOS

// These extern directives let us access the addresses of our ASM ISR handlers.
extern "C" void isr0 ();
extern "C" void isr1 ();
extern "C" void isr2 ();
extern "C" void isr3 ();
extern "C" void isr4 ();
extern "C" void isr5 ();
extern "C" void isr6 ();
extern "C" void isr7 ();
extern "C" void isr8 ();
extern "C" void isr9 ();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

#endif // _MOROS_IDT_H

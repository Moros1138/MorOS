#ifndef _MOROS_IDT_H
#define _MOROS_IDT_H

#include "types.h"
#include "io.h"
#include "isr.h"

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





#endif // _MOROS_IDT_H

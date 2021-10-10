#include "idt.h"

namespace MorOS
{
    InterruptManager::GateDescriptor::GateDescriptor()
    {
        base_lo   = 0;
        base_hi   = 0;
        sel       = 0;
        always0   = 0;
        flag_byte = 0;
    }

    InterruptManager::GateDescriptor::GateDescriptor(uint32_t base, uint16_t selector, uint8_t flags)
    {
        Set(base, selector, flags);
    }

    void InterruptManager::GateDescriptor::Set(uint32_t base, uint16_t selector, uint8_t flags)
    {
        base_lo = base & 0xFFFF;
        base_hi = (base >> 16) & 0xFFFF;
        sel     = selector;
        always0 = 0;
        flag_byte   = flags /* | 0x60 */;
    }

    InterruptManager::InterruptManager()
    {
        uint16_t idt_ptr[3];
        
        idt_ptr[0] = sizeof(MorOS::InterruptManager::GateDescriptor) * 256 - 1;
        idt_ptr[2] = ((uint32_t)&interrupts >> 16) & 0x0000ffff;
        idt_ptr[1] = ((uint32_t)&interrupts) & 0x0000ffff;
        
        interrupts[ 0].Set((uint32_t)isr0 , 0x08, 0x8E);
        interrupts[ 1].Set((uint32_t)isr1 , 0x08, 0x8E);
        interrupts[ 2].Set((uint32_t)isr2 , 0x08, 0x8E);
        interrupts[ 3].Set((uint32_t)isr3 , 0x08, 0x8E);
        interrupts[ 4].Set((uint32_t)isr4 , 0x08, 0x8E);
        interrupts[ 5].Set((uint32_t)isr5 , 0x08, 0x8E);
        interrupts[ 6].Set((uint32_t)isr6 , 0x08, 0x8E);
        interrupts[ 7].Set((uint32_t)isr7 , 0x08, 0x8E);
        interrupts[ 8].Set((uint32_t)isr8 , 0x08, 0x8E);
        interrupts[ 9].Set((uint32_t)isr9 , 0x08, 0x8E);
        interrupts[10].Set((uint32_t)isr10, 0x08, 0x8E);
        interrupts[11].Set((uint32_t)isr11, 0x08, 0x8E);
        interrupts[12].Set((uint32_t)isr12, 0x08, 0x8E);
        interrupts[13].Set((uint32_t)isr13, 0x08, 0x8E);
        interrupts[14].Set((uint32_t)isr14, 0x08, 0x8E);
        interrupts[15].Set((uint32_t)isr15, 0x08, 0x8E);
        interrupts[16].Set((uint32_t)isr16, 0x08, 0x8E);
        interrupts[17].Set((uint32_t)isr17, 0x08, 0x8E);
        interrupts[18].Set((uint32_t)isr18, 0x08, 0x8E);
        interrupts[19].Set((uint32_t)isr19, 0x08, 0x8E);
        interrupts[20].Set((uint32_t)isr20, 0x08, 0x8E);
        interrupts[21].Set((uint32_t)isr21, 0x08, 0x8E);
        interrupts[22].Set((uint32_t)isr22, 0x08, 0x8E);
        interrupts[23].Set((uint32_t)isr23, 0x08, 0x8E);
        interrupts[24].Set((uint32_t)isr24, 0x08, 0x8E);
        interrupts[25].Set((uint32_t)isr25, 0x08, 0x8E);
        interrupts[26].Set((uint32_t)isr26, 0x08, 0x8E);
        interrupts[27].Set((uint32_t)isr27, 0x08, 0x8E);
        interrupts[28].Set((uint32_t)isr28, 0x08, 0x8E);
        interrupts[29].Set((uint32_t)isr29, 0x08, 0x8E);
        interrupts[30].Set((uint32_t)isr30, 0x08, 0x8E);
        interrupts[31].Set((uint32_t)isr31, 0x08, 0x8E);


        // remap irq table
        outb(0x20, 0x11);
        outb(0xA0, 0x11);
        outb(0x21, 0x20);
        outb(0xA1, 0x28);
        outb(0x21, 0x04);
        outb(0xA1, 0x02);
        outb(0x21, 0x01);
        outb(0xA1, 0x01);
        outb(0x21, 0x0);
        outb(0xA1, 0x0);

        interrupts[32].Set((uint32_t)irq0 , 0x08, 0x8E);
        interrupts[33].Set((uint32_t)irq1 , 0x08, 0x8E);
        interrupts[34].Set((uint32_t)irq2 , 0x08, 0x8E);
        interrupts[35].Set((uint32_t)irq3 , 0x08, 0x8E);
        interrupts[36].Set((uint32_t)irq4 , 0x08, 0x8E);
        interrupts[37].Set((uint32_t)irq5 , 0x08, 0x8E);
        interrupts[38].Set((uint32_t)irq6 , 0x08, 0x8E);
        interrupts[39].Set((uint32_t)irq7 , 0x08, 0x8E);
        interrupts[40].Set((uint32_t)irq8 , 0x08, 0x8E);
        interrupts[41].Set((uint32_t)irq9 , 0x08, 0x8E);
        interrupts[42].Set((uint32_t)irq10, 0x08, 0x8E);
        interrupts[43].Set((uint32_t)irq11, 0x08, 0x8E);
        interrupts[44].Set((uint32_t)irq12, 0x08, 0x8E);
        interrupts[45].Set((uint32_t)irq13, 0x08, 0x8E);
        interrupts[46].Set((uint32_t)irq14, 0x08, 0x8E);
        interrupts[47].Set((uint32_t)irq15, 0x08, 0x8E);

        idt_flush((uint32_t)&idt_ptr);
    }

} // MorOS

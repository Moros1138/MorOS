#include "isr.h"
isr_t interrupt_handlers[256];

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    MorOS::printf("Recieved Interrupt: %d\n", regs.int_no);
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{

    isr_t handler = interrupt_handlers[regs.int_no];
    if(handler)
    {
        handler(regs);
    }
    
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        MorOS::outb(0xA0, 0x20);
    }

    // Send reset signal to master. (As well as slave, if necessary).
    MorOS::outb(0x20, 0x20);


}


void irq_set_mask(uint32_t i)
{
    uint16_t port = i < 8 ? 0x21 : 0xA1;
    uint8_t value = MorOS::inb(port) | (1 << i);

    MorOS::outb(port, value);
}

void irq_clear_mask(uint32_t i)
{
    uint16_t port = i < 8 ? 0x21 : 0xA1;
    uint8_t value = MorOS::inb(port) & ~(1 << i);
    MorOS::outb(port, value);
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    asm volatile("cli;");
    interrupt_handlers[n] = handler;
    irq_clear_mask(n);
    asm volatile("sti;");
}

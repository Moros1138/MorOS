#include "hardware/vga.h"
#include "hardware/cpu.h"
#include "hardware/interrupts.h"
#include "hardware/io.h"

#include "stdlib.h"
#include "string.h"


extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

void gdt_init();
void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

void idt_init();
void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void init_descriptor_tables()
{
    gdt_init();
    idt_init();
}

void gdt_init()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base =  (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);
    gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);
    gdt_set_gate(3, 0, 0xffffffff, 0xfa, 0xcf);
    gdt_set_gate(4, 0, 0xffffffff, 0xf2, 0xcf);

    gdt_flush((uint32_t)&gdt_ptr);
}

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low     = (base & 0xffff);
    gdt_entries[num].base_middle  = (base >> 16) & 0xff;
    gdt_entries[num].base_high    = (base >> 24) & 0xff;
    
    gdt_entries[num].limit_low    = (limit & 0xffff);
    gdt_entries[num].granularity  = (limit >> 16) & 0x0f;
    gdt_entries[num].granularity |= gran & 0xf0;
    gdt_entries[num].access       = access;
}

void idt_init()
{
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
  
    // Remap the irq table.
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

    idt_set_gate(IRQ0 , (uint32_t)irq0 , 0x08, 0x8E);
    idt_set_gate(IRQ1 , (uint32_t)irq1 , 0x08, 0x8E);
    idt_set_gate(IRQ2 , (uint32_t)irq2 , 0x08, 0x8E);
    idt_set_gate(IRQ3 , (uint32_t)irq3 , 0x08, 0x8E);
    idt_set_gate(IRQ4 , (uint32_t)irq4 , 0x08, 0x8E);
    idt_set_gate(IRQ5 , (uint32_t)irq5 , 0x08, 0x8E);
    idt_set_gate(IRQ6 , (uint32_t)irq6 , 0x08, 0x8E);
    idt_set_gate(IRQ7 , (uint32_t)irq7 , 0x08, 0x8E);
    idt_set_gate(IRQ8 , (uint32_t)irq8 , 0x08, 0x8E);
    idt_set_gate(IRQ9 , (uint32_t)irq9 , 0x08, 0x8E);
    idt_set_gate(IRQ10, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(IRQ11, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(IRQ12, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(IRQ13, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(IRQ14, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(IRQ15, (uint32_t)irq15, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
}

isr_t interrupt_handlers[256];

char* interrupt_names[] = {
    "Divide-by-zero Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "reserved",
    "Hypervisor Injection Expection",
    "VMM Communication Exception",
    "Security Exception",
    "reserved",
    "Triple Fault",
    "FPU Error Interrupt"
};

void isr_handler(registers_t regs)
{
    vga_set_mode(TextMode);

    printf("                                                        \n");
    printf("  ####  #    #      ####      ### ####   #    # ##  ##  \n");
    printf(" #    # #    #      #   #    #  # #    # ##   # ##  ##  \n");
    printf(" #    # #    #      #    #  #   # #    # # #  # ##  ##  \n");
    printf(" #    # ######      #    # #   ## #    # # #  # ##  ##  \n");
    printf(" #    # ######      #    # ###### #####  #  # # ##  ##  \n");
    printf(" #    # #    #      #    # #    # # #    #  # #         \n");
    printf(" #    # #    #      #   #  #    # #  ##  #   ## ##  ##  \n");
    printf("  ####  #    # ##   ####   #    # #    # #    #  #  #   \n");
    printf("                #                                       \n\n");

    printf("%s (0x%08x)\n\n", interrupt_names[regs.int_no], regs.err_code);
    printf("THE INFO:\n\n");
    printf("CS      : 0x%08x        DS    : 0x%08x\n", regs.cs, regs.ds);
    printf("EDI     : 0x%08x        ESI   : 0x%08x\n", regs.edi, regs.esi);
    printf("EBP     : 0x%08x        ESP   : 0x%08x\n", regs.ebp, regs.esp);
    printf("EAX     : 0x%08x        EBX   : 0x%08x\n", regs.eax, regs.ebx);
    printf("ECX     : 0x%08x        EDX   : 0x%08x\n", regs.ecx, regs.edx);

    printf("EIP     : 0x%08x        EFLAGS: 0x%08x\n", regs.eip, regs.eflags);
    printf("USERESP : 0x%08x        SS    : 0x%08x\n", regs.useresp, regs.ss);

    while(1);
}

void irq_handler(registers_t regs)
{
    // send EOI if interrupt involved the slave
    if(regs.int_no >= 40)
        outb(0xa0, 0x20);
    
    // send reset signal to master, and slave if needed
    outb(0x20, 0x20);

    isr_t handler = interrupt_handlers[regs.int_no];
    if(handler)
    {
        __asm__ __volatile__("cli;");
        handler(regs);
        __asm__ __volatile__("sti;");
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

#include "isr.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    MorOS::printf("Recieved Interrupt: %d\n", regs.int_no);
}

#include "keyboard.h"


namespace MorOS
{

    unsigned char kbdus[128] =
    {
        0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
        '\t','q','w','e','r','t', 'y', 'u', 'i', 'o', 'p', '[', ']',
        '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
        '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
        '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0,
    };

    uint16_t Keyboard::last_key = 0;
    
    Keyboard::Keyboard()
    {
        register_interrupt_handler(IRQ1, &MorOS::Keyboard::handler);
    }

    void Keyboard::handler(registers_t regs)
    {

        uint8_t scancode;

        /* Read from the keyboard's data buffer */
        scancode = inb(0x60);

        /* If the top bit of the byte we read from the keyboard is
        *  set, that means that a key has just been released */
        if(scancode & 0x80)
        {
            /* You can use this one to see if the user released the
            *  shift, alt, or control keys... */
        }
        else
        {
            Monitor::activeMonitor->putc(kbdus[scancode]);
        }
    }
}
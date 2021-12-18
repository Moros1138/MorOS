#include "hardware/keyboard.h"
#include "hardware/io.h"
#include "hardware/interrupts.h"

#include "stdlib.h"
#include "string.h"
#include "event.h"

// SCANCODE SET 1
// https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1
static char keys[2][128] =
{
    {
        0,27,'1','2','3','4','5','6','7','8','9','0','-',
        '=',8,'\t','q','w','e','r','t','y','u','i','o','p',
        '[',']','\n',0,'a','s','d','f','g','h','j','k','l',
        ';','\'','`',0,'\\','z','x','c','v','b','n','m',',',
        '.','/',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,'7',
        '8','9','-','4','5','6','+','1','2','3','0','.',0,
        0,0,0,0
    },
    {
        0,27,'!','@','#','$','%','^','&','*','(',')','_',
        '+',8,'\t','Q','W','E','R','T','Y','U','I','O','P',
        '{','}','\n',0,'A','S','D','F','G','H','J','K','L',
        ':','"' ,'~',0,'|' ,'Z','X','C','V','B','N','M','<',
        '>','?',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,'7',
        '8','9','-','4','5','6','+','1','2','3','0','.',0,
        0,0,0,0
    }
};

static bool isExtendedKey;

static void keyboard_interrupt_handler(registers_t regs)
{
    UNUSED(regs);
    
    // read scancode from keyboard
    uint32_t scancode = inb(0x60);

    // if extended key, set flag and discard
    if(scancode == 0xe0)
    {
        isExtendedKey = true;
        return;
    }
    
    key_event_t key;
    key.type = (scancode & 0x80) ? KeyRelease : KeyPress;
    key.keycode = (scancode & 0x7f);
    key.scancode = scancode;

    if(isExtendedKey)
    {
        key.scancode |= 0xe000;
        key.keycode += 0x80;
    }
    
    event_fire((event_t*)&key);
    
    // reset extended flag
    isExtendedKey = false;
}

int keyboard_init()
{
    UNUSED(keys);
    
    // initialize variables
    isExtendedKey = false;

    // register keyboard interrupt handler
    register_interrupt_handler(IRQ1, &keyboard_interrupt_handler);

    return 1;
}

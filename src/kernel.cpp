#include "multiboot.h"

#include "hardware/io.h"
#include "hardware/cpu.h"
#include "hardware/timer.h"
#include "hardware/keyboard.h"
#include "hardware/mouse.h"
#include "hardware/vga.h"
#include "hardware/serial.h"

#include "mem.h"
#include "event.h"
#include "stdlib.h"
#include "assert.h"

extern "C" int kernel_setup(multiboot_info_t* mbd, unsigned int magic)
{
    assert(magic == 0x2badb002);
    
    init_descriptor_tables();
    memory_init(mbd);

    event_init();

    vga_init();
    serial_init();
    timer_init();
    keyboard_init();
    mouse_init();

    srand(0);

    return 0x3badb002;
}

void print_event(event_t* e)
{
    printf("TYPE: %s ", event_names[e->type]);

    if(e->type == KeyPress || e->type == KeyRelease)
        printf(" | Scancode(0x%04x) Keycode(0x%02x) | ", e->key.scancode, e->key.keycode);
    
    if(e->type == ButtonPress || e->type == ButtonRelease)
        printf(" | Button(%d) | ", e->button.button);
    
    printf("\n");
}

extern "C" void kernel_main()
{
    while(1)
    {
        while(event_pending())
        {
            event_t e;
            event_next(&e);
            print_event(&e);
        }
    }
}

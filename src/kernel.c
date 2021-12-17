#include "multiboot.h"

#include "hardware/io.h"
#include "hardware/cpu.h"
#include "hardware/fpu.h"
#include "hardware/timer.h"
#include "hardware/keyboard.h"
#include "hardware/mouse.h"
#include "hardware/vga.h"
#include "hardware/serial.h"

#include "mem.h"
#include "event.h"

#include <stdlib.h>
#include <assert.h>


int kernel_setup(multiboot_info_t* mbd, unsigned int magic)
{
    assert(magic == 0x2badb002);
    
    init_descriptor_tables();
    
    memory_init(mbd);
    event_init();

    serial_init();
    vga_init();
    
    if(!fpu_init())
        __asm__ __volatile__("mov $0xdead0001, %edx; int $0x1;");

    timer_init();       // IRQ0
    
    if(!keyboard_init()) // IRQ1
        __asm__ __volatile__("mov $0xdead0002, %edx; int $0x1;");
    
    if(!mouse_init()) // IRQ12
        __asm__ __volatile__("mov $0xdead0003, %edx; int $0x1;");

    // currently not used, but might be useful later for debugging
    return 0x3badb002;
}

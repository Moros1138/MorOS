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
    timer_init();
    keyboard_init();
    mouse_init();
    
    // vga_mode13();
    fpu_init();

    return 0x3badb002;
}

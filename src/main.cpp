#include "multiboot.h"

#include "types.h"

#include "descriptors/gdt.h"
#include "descriptors/idt.h"
#include "hardware/monitor.h"
#include "hardware/timer.h"
#include "hardware/keyboard.h"
#include "memory.h"
#include "random.h"

using namespace MorOS;

extern "C" void _main(multiboot_info_t* mbd, uint32_t)
{
    // global descriptor table
    MorOS::GlobalDescriptorTable gdt{};
    
    // interrupts
    MorOS::InterruptManager interruptManager{};

    // approximately 30 fps
    // MorOS::Timer(65355);

    // monitor (VGA mode 13h)
    MorOS::Monitor monitor{};

    // memory
    MorOS::MemoryManager memoryManager(mbd);

    // randomizer
    MorOS::Random random{};

    // keyboard
    MorOS::Keyboard keyboard{};
    
    // ~1000 fps - ~1.00ms per tick -  1193
    MorOS::Timer(1193);
    
    uint32_t tp1 = MorOS::Timer::tick;
    uint32_t tp2 = MorOS::Timer::tick;

    // just chill out forever!
    while(1)
    {
        tp2 = MorOS::Timer::tick;
        int32_t ticks = tp2 - tp1;
        tp1 = tp2;
        
        printf("\n--- %d timer ticks between frames---\n", ticks);
        for(int i = 0; i < 500000000; i++)
            asm("nop;");
    }
}

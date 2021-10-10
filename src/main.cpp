#include "multiboot.h"

#include "types.h"
#include "monitor.h"
#include "gdt.h"
#include "idt.h"
#include "memory.h"

extern "C" void _main(multiboot_info_t* mbd, uint32_t)
{
    // our global monitor
    MorOS::Monitor monitor{};
    MorOS::GlobalDescriptorTable gdt{};
    MorOS::InterruptManager interruptManager{};
    
    multiboot_memory_map_t* mmmt = 0;
    
    // cycle through the memory map in search of the memory chunk at 1MiB
    for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_high == 0 &&
            mmmt->addr_low  == 0x100000)
        {
            break;
        }
    }

    // if we have a memory chunk at 1MiB, let's totally use it!
    if(mmmt != 0)
        MorOS::MemoryManager memoryManager(mmmt->addr_low, mmmt->len_low);

    // moment of fucking truth
    asm volatile("int $0x04;");
    asm volatile("int $0x05;");
    asm volatile("int $0x06;");
    asm volatile("int $0x07;");

    // hang out here forever
}
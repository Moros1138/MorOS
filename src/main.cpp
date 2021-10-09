#include "multiboot.h"

#include "types.h"
#include "monitor.h"




extern "C" void _main(multiboot_info_t* mbd, uint32_t)
{
    MorOS::Monitor monitor{};
    for(int i = 0; i < 600; i++)
        monitor.puts("Hi. ");
    
    monitor.puts("\n\nLast line....");
    while(1);
}
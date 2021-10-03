/*
    believe it or not, these are provided by the compiler
    even though we're not actually using any of the standard
    libraries. saves a lot of hassle and allows me to use
    some types I'm used to using!
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <Terminal.h>
#include <MemoryManager.h>
#include <Randomizer.h>

using namespace MorOS;

#define delay(duration) for(int i = 0; i < duration; i++) { __asm__("nop"); }

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

/*
    this is labelled as extern "C" to prevent the C++ compiler from
    mangling the function name. this allows it to be called from
    the boot.s this doesn't stop us from using C++ features within
    this function, however. I'll have fun with this later on, I'm
    certain of it.
*/
extern "C" void kmain(multiboot_info_t* mbd, uint32_t magic)
{
    // our global terminal
    Terminal terminal{};

    // our global randomizer
    Randomizer randomizer{};

    multiboot_memory_map_t* mmmt = 0;

    for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) 
    {
        mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_high == 0 &&
            mmmt->addr_low == 0x100000)
        {
            break;
        }
    }


    // our global memory manager
    if(mmmt != 0)
        MemoryManager memoryManager(mmmt->addr_low, mmmt->len_low);
        

    // // memory information provided by mutliboot specification
    // size_t max_memory = mbd->mem_upper * 1024;
    
    // // 10 MiB pre-calculated
    // size_t heap_size = 10 * 1024 * 1024;

    // // basically, give me 10 MiB of ram, pretty please
    // uint8_t* heap_memory = (uint8_t*)(max_memory - heap_size - 10*1024);
    // char buf[256] = { 0 };

    // puts("MorOS - Max  Memory: ");
    // itoa(buf, 'x', max_memory);
    // puts(buf);
    // puts("\n");

    // puts("MorOS - Heap Memory: ");
    // itoa(buf, 'x', (size_t)heap_memory);
    // puts(buf);
    // puts("\n");

    // puts("MorOS - Heap Size:   ");
    // itoa(buf, 'x', heap_size);
    // puts(buf);
    // puts("\n");

    // char buf[] = "Hello, World!\n";
    // strcpy((char*)heap_memory, buf);

    // puts("\nThe following is contained within the heap memory:\n\n");
    // puts((char*)heap_memory);
    // puts("\n");

    // let's hang out here forever!
    while(1)
    { 
    }
}
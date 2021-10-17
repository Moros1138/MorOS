#include "memory.h"

using namespace MorOS;

namespace MorOS
{

    MemoryManager* MemoryManager::activeMemoryManager = 0;
    
    MemoryManager::MemoryManager()
    {
        activeMemoryManager = this;
    }

    MemoryManager::MemoryManager(multiboot_info_t* mbd)
    {
        activeMemoryManager = this;
        
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
        {
            start = mmmt->addr_low;
            end = mmmt->addr_low + mmmt->len_low;  
        }
        
        printf("    --- MEMORY MANAGER ---\n");
        printf("Start: 0x%x - %d\n", start, start);
        printf("End:   0x%x - %d\n", end, end);
    }
    
    MemoryManager::MemoryManager(size_t address, size_t size)
    {
        activeMemoryManager = this;
        start = address;
        end = address + size;
        
        printf("    --- MEMORY MANAGER ---\n");
        printf("Start: 0x%x - %d\n", start, start);
        printf("End:   0x%x - %d\n", end, end);
    }
    
    MemoryManager::~MemoryManager()
    {

    }

    void* malloc(size_t size)
    {
        (void)(size);
        return 0;
    }

    void free(void* mem)
    {
        (void)(mem);
    }

    // copy len bytes from src to dest.
    void memcpy(void *dest, void* src, size_t len)
    {
        uint8_t* pTempDest = (uint8_t*)dest;
        uint8_t* pTempSrc = (uint8_t*)src;

        for(size_t i; i < len; i++)
            pTempDest[i] = pTempSrc[i];
    }

    // write len copies of val into dest.
    void memset(void* dest, uint8_t val, size_t len)
    {
        uint8_t* pTempDest = (uint8_t*)dest;
        
        for(size_t i = 0; i < len; i++)
            pTempDest[i] = val;
    }

} // MorOS
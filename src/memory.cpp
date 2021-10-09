#include "memory.h"

using namespace MorOS;

namespace MorOS
{

    MemoryManager* MemoryManager::activeMemoryManager = 0;

    MemoryManager::MemoryManager()
    {
        activeMemoryManager = this;
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

    // Copy len bytes from src to dest.
    void memcpy(void *dest, void* src, uint32_t len)
    {
        const uint8_t *sp = (const uint8_t*)src;
        uint8_t* dp = (uint8_t*)dest;
        for(; len != 0; len--) *dp++ = *sp++;
    }

    // Write len copies of val into dest.
    void memset(void* dest, uint8_t val, uint32_t len)
    {
        uint8_t* temp = (uint8_t*)dest;
        for ( ; len != 0; len--) *temp++ = val;
    }

} // MorOS
#ifndef _MOROS_MEMORY_MANAGER_H
#define _MOROS_MEMORY_MANAGER_H

#include "types.h"
#include "monitor.h"

namespace MorOS
{

    class MemoryManager
    {
    public:
        MemoryManager();
        MemoryManager(size_t address, size_t size);
        ~MemoryManager();

        static MemoryManager* activeMemoryManager;
    
    private:
        size_t start;
        size_t end;
    };

    void* malloc(size_t size);
    void free(void* mem);
    
    void memcpy(void* dest, void* src, uint32_t len);
    void memset(void* dest, uint8_t val, uint32_t len);


} // MorOS

#endif // _MOROS_MEMORY_MANAGER_H
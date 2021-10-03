#ifndef _MOROS_MEMORY_MANAGER_H
#define _MOROS_MEMORY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <Terminal.h>
#include <multiboot.h>

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

} // MorOS

#endif // _MOROS_MEMORY_MANAGER_H
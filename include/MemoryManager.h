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
        MemoryManager(multiboot_info_t* mbd);
        ~MemoryManager();

        static MemoryManager* activeMemoryManager;
    };

    void* malloc(size_t size);
    void free(void* mem);

} // MorOS

#endif // _MOROS_MEMORY_MANAGER_H
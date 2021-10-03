#include <MemoryManager.h>

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
        printf("Start:   %08lx\n", start);
        printf("End:     %08lx\n", end);
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

} // MorOS
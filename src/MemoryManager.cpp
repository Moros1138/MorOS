#include <MemoryManager.h>

using namespace MorOS;

namespace MorOS
{

    MemoryManager* MemoryManager::activeMemoryManager = 0;

    MemoryManager::MemoryManager()
    {
        activeMemoryManager = this;
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
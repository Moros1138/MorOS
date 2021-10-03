#include <MemoryManager.h>

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
        for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) 
        {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);

    
            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
            {
                // do something with this memory block!

                puts("    --- BEGIN ENTRY ---\n");
                puts("Start Addr: ");
                puthex(mmmt->addr_high);
                puthex(mmmt->addr_low);
                puts("\n");
                puts("Length: ");
                puthex(mmmt->len_high);
                puthex(mmmt->len_low);
                puts("\n");
                puts("Size: ");
                puthex(mmmt->size);
                puts("\n");
                puts("Type: ");
                puthex(mmmt->type);
                puts("\n");
                puts("    --- END   ENTRY ---\n");
            }
        }

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
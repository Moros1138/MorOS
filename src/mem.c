#include "mem.h"

#include "assert.h"
#include "string.h"

struct memory_block
{
    struct memory_block* prev;
    struct memory_block* next;
    size_t size;
    bool allocated;
};
typedef struct memory_block memory_block_t;

memory_block_t* __MEMORY_BLOCK_BEGIN;

void memory_init(multiboot_info_t* mbd)
{
    // set first block to null
    __MEMORY_BLOCK_BEGIN = NULL;

    // memory map provided by multiboot specification
    multiboot_memory_map_t* memoryMap = (multiboot_memory_map_t*)mbd->mmap_addr;
    
    // get the size of the memory map entries
    size_t memoryMapCount = mbd->mmap_length / sizeof(multiboot_memory_map_t);
    
    size_t i;
    
    // cycle through the memory map entries until we find the one at 1MB
    for(i = 0; i < memoryMapCount; i++)
    {
        if(memoryMap[i].type == 1 && memoryMap[i].addr == 0x100000)
        {
            __MEMORY_BLOCK_BEGIN = ((memory_block_t*)((size_t)memoryMap[i].addr + 0xa00000));
            __MEMORY_BLOCK_BEGIN->allocated = false;
            __MEMORY_BLOCK_BEGIN->prev = NULL;
            __MEMORY_BLOCK_BEGIN->next = NULL;
            __MEMORY_BLOCK_BEGIN->size = (size_t)memoryMap[i].len - sizeof(memory_block_t);
            break;
        }
    }
    
    assert(__MEMORY_BLOCK_BEGIN != NULL);
}

void* malloc(size_t size)
{
    // give us a place to store the result
    memory_block_t* result = NULL;

    // see if we have an existing block with which we can use
    for(memory_block_t* block = __MEMORY_BLOCK_BEGIN; block != NULL && result == NULL; block = block->next)
        if(block->size > size && !block->allocated)
            result = block;

    // if we make it here and result is still NULL, something bad has happened.
    if(result == NULL)
        return NULL;

    // if the resulting block is bigger than we need, let's split it up
    if(result->size >= size + sizeof(memory_block_t) + 1)
    {
        memory_block_t* temp = (memory_block_t*)((size_t)result + sizeof(memory_block_t) + size);
        
        temp->allocated = false;
        // calculate the size of the next block
        temp->size = result->size - size - sizeof(memory_block_t);

        // insert the new chunk into the list
        temp->prev = result;
        temp->next = result->next;
        
        if(temp->next != NULL)
            temp->next->prev = temp;

        result->size = size;
        result->next = temp;    
    }
    
    // we're allocated
    result->allocated = true;
    
    // return the pointer to the raw data
    return result+1;
}

void free(void* ptr)
{
    memory_block_t* block = (memory_block_t*)((size_t)ptr - sizeof(memory_block_t));
    
    // no longer allocated
    block->allocated = false;
    
    // merge with previous block, if it exists and is not allocated
    if(block->prev != NULL && !block->prev->allocated)
    {
        block->prev->next = block->next;
        block->prev->size += block->size + sizeof(memory_block_t);
        if(block->next != NULL)
            block->next->prev = block->prev;

        block = block->prev;
    }

    // merge with next block, if it exists and is not allocated
    if(block->next != NULL && !block->next->allocated)
    {
        block->size += block->next->size + sizeof(memory_block_t);
        block->next = block->next->next;
        if(block->next != NULL)
            block->next->prev = block;
    }
    
}

// TODO
//      this is stupid, does not get smaller.
//      if bigger, full malloc of new size.
//      works in general, but is not efficient and
//      in many ways defeats the purpose of using it
void* realloc(void* ptr, size_t new_size)
{
    if(ptr == NULL)
        return malloc(new_size);

    memory_block_t* block = (memory_block_t*)ptr-1;
    
    // if our allocation can fit the new size, return the pointer
    if(block->size >= new_size)
        return ptr;

    // if we make it here, we need a new allocation    
    void* temp = malloc(new_size);
    
    // did something go horribly wrong?
    if(temp == NULL)
        return NULL;

    memcpy(temp, ptr, block->size);
    free(ptr);
        
    return temp;
}

void* calloc(size_t num, size_t size)
{
    void* ret = malloc(num * size);
    
    if(ret == NULL)
        return NULL;

    memset(ret, 0, num * size);
    return ret;
}

void print_memory_blocks()
{
    memory_block_t* cur = __MEMORY_BLOCK_BEGIN;

    printf(" ---------------- BEGIN MEMORY BLOCKS ----------------\n");
    while(cur != NULL)
    {
        printf("  %s Addr(0x%08lx) Size( % 10lu ) Prev(0x%08lx) Next(0x%08lx)\n", (cur->allocated) ? "-USED-" : "-FREE-", (size_t)cur, cur->size, cur->prev, cur->next);
        cur = cur->next;
    }
    printf(" ---------------- END MEMORY BLOCKS   ----------------\n\n");
}

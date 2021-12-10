#include "stdlib.h"

void* operator new(size_t size)
{
    void* p = malloc(size);
    return p;
}

void* operator new[](size_t size)
{
    void* p = malloc(size);
    return p;
}

void operator delete(void* p)
{
    free(p);
}

void operator delete(void* p, size_t size)
{
    UNUSED(size);
    free(p);
}

void operator delete[](void* p)
{
    free(p);
}

void operator delete[](void* p, size_t size)
{
    UNUSED(size);
    free(p);
}

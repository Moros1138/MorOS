typedef unsigned long size_t;

extern "C" void* malloc(size_t size);
extern "C" void free(void *p);

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

void* operator new(size_t, void* __p)
{
    return __p;
}

void* operator new[](size_t, void* __p)
{
    return __p;
}

void operator delete(void* p)
{
    free(p);
}

void operator delete(void* p, size_t size)
{
    (void)(size);
    free(p);
}

void operator delete[](void* p)
{
    free(p);
}

void operator delete[](void* p, size_t size)
{
    (void)(size);
    free(p);
}

void operator delete  (void*, void*)
{

}

void operator delete[](void*, void*)
{

}

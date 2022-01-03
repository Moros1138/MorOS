// -*- C++ -*-
#ifndef _CXX_MOROS_ALLOCATOR_H
#define _CXX_MOROS_ALLOCATOR_H

#include "moros_config.h"

#if !defined(__MOROS_KERNEL__)
#   include <stdio.h>
#   include <stdlib.h>
#   include <assert.h>
#else
#   include "assert.h"
#   include "stdlib.h"
#endif

#include "moros_config.h"

namespace __MOROS_NAMESPACE__
{
    template <typename T>
    class allocator 
    {
    public:
        typedef T  value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;

        allocator()
        {
            MOROS_INFO("MorOS::allocator::allocator();\n");
        }

        ~allocator()
        {
            MOROS_INFO("MorOS::allocator::~allocator();\n");
        }

        pointer allocate(size_type n, const void* hint = 0)
        {
            MOROS_INFO("MorOS::allocate(%ld, %ld) - ", n, (size_type)hint);
            (void)hint;
            
            if(n < 1)
                return 0;
            
            pointer ret = (pointer)malloc(n * sizeof(value_type));
            MOROS_INFO("pointer(0x%016lx)... success\n", (size_type)ret);

            return ret;
        }

        void deallocate(pointer p, size_type n)
        {
            (void)(n);
            free(p);
        }
    };


} // MorOS


#endif // _CXX_MOROS_ALLOCATOR_H


// -*- C++ -*-
#ifndef _CXX_MOROS_ALLOCATOR_H
#define _CXX_MOROS_ALLOCATOR_H

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#   include <stdio.h>
#   include <assert.h>
#else
#   include "assert.h"
#   include "stdlib.h"
#endif

#include "moros_config.h"

namespace MorOS
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
           MOROS_DEBUG("MorOS::allocator()\n");
        }

        ~allocator()
        {
            MOROS_DEBUG("MorOS::~allocator()\n");
        }

        pointer allocate(size_type n, const void* hint = 0)
        {
            (void)hint;
            
            if(n < 1)
                return 0;
            
            pointer ret = new value_type[n];
            MOROS_DEBUG("MorOS::allocate(%ld, %ld) - pointer(%ld)\n", n, (size_type)hint, (size_type)ret);
            return ret;
        }

        void deallocate(pointer p, size_type n)
        {
            MOROS_DEBUG("MorOS::deallocate(%ld, %ld)\n", (size_type)p, n);
            delete[] p;
        }
    };


} // MorOS


#endif // _CXX_MOROS_ALLOCATOR_H


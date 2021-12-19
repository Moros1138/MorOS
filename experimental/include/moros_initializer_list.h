// -*- C++ -*-
#ifndef _CXX_MOROS_INITIALIZER_LIST_H
#define _CXX_MOROS_INITIALIZER_LIST_H

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#   include <stdio.h>
#   include <assert.h>
#else
#   include "assert.h"
#   include "stdlib.h"
#endif

namespace MorOS
{

template<class T>
class initializer_list
{
    typedef T value_type;
    typedef const T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    
    typedef const T* iterator;
    typedef const T* const_iterator;

    const_iterator _begin;
    size_type _size;
    
    inline constexpr initializer_list(const_iterator _b, size_type _s) noexcept
        : _begin(_b),
          _size(_s)
    {}
    
public:
    
    inline
    constexpr
    initializer_list() noexcept : __begin_(nullptr), __size_(0) {}
    
    inline
    constexpr
    size_t    size()  const noexcept {return __size_;}
    
    inline
    constexpr
    const _Ep* begin() const noexcept {return __begin_;}
    
    inline
    constexpr
    const _Ep* end()   const noexcept {return __begin_ + __size_;}
};

template<class _Ep>
inline
constexpr
const _Ep*
begin(initializer_list<_Ep> __il) noexcept
{
    return __il.begin();
}

template<class _Ep>
inline
constexpr
const _Ep*
end(initializer_list<_Ep> __il) noexcept
{
    return __il.end();
}


} // MorOS

#endif // _CXX_MOROS_INITIALIZER_LIST_H
// -*- C++ -*-
#ifndef _CXX_MOROS_UTILITY_H
#define _CXX_MOROS_UTILITY_H

#include "moros_type_traits.h"

namespace MorOS
{

template <class T>
inline T&& forward(typename MorOS::remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
inline T&& forward(typename MorOS::remove_reference<T>::type&& t) noexcept
{
    // static_assert(!std::is_lvalue_reference<T>::value,
    //               "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

} // MorOS

#endif // _CXX_MOROS_UTILITY_H

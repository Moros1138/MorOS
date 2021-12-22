// -*- C++ -*-
#ifndef _CXX_MOROS_UTILITY_H
#define _CXX_MOROS_UTILITY_H

#include "moros_config.h"
#include "moros_type_traits.h"

namespace __MOROS_NAMESPACE__
{

template <class T>
inline T&& forward(typename __MOROS_NAMESPACE__::remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
inline T&& forward(typename __MOROS_NAMESPACE__::remove_reference<T>::type&& t) noexcept
{
    // static_assert(!std::is_lvalue_reference<T>::value,
    //               "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

} // __MOROS_NAMESPACE__

#endif // _CXX_MOROS_UTILITY_H

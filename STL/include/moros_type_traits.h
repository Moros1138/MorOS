// -*- C++ -*-
#ifndef _CXX_MOROS_TYPE_TRAITS_H
#define _CXX_MOROS_TYPE_TRAITS_H

#include "moros_config.h"

namespace __MOROS_NAMESPACE__
{

template <class T>
struct remove_reference { typedef T type; };

template <class T>
struct remove_reference<T&> { typedef T type; };

template <class T>
struct remove_reference<T&&> { typedef T type; };

} // __MOROS_NAMESPACE__

#endif // _CXX_MOROS_TYPE_TRAITS_H

// -*- C++ -*-
#ifndef _CXX_MOROS_TYPE_TRAITS_H
#define _CXX_MOROS_TYPE_TRAITS_H

namespace MorOS
{

template <class T>
struct remove_reference { typedef T type; };

template <class T>
struct remove_reference<T&> { typedef T type; };

template <class T>
struct remove_reference<T&&> { typedef T type; };

} // MorOS

#endif // _CXX_MOROS_TYPE_TRAITS_H

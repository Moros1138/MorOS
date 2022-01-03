// -*- C++ -*-
#ifndef _CXX_MOROS_ALGORITHM_H
#define _CXX_MOROS_ALGORITHM_H

#include "moros_config.h"

namespace __MOROS_NAMESPACE__
{
    template<typename T>
    inline const T& swap(const T& __a, const T& __b)
    { const T __t = __a; __b = __a; __a = __t; }

    template<typename T>
    inline const T& max(const T& __a, const T& __b)
    {
        if(__a < __b) return __b;
        return __a;
    }

    template<typename T>
    inline const T& min(const T& __a, const T& __b)
    {
        if(__a > __b) return __b;
        return __a;
    }
} // std

#endif // _CXX_MOROS_ALGORITHM_H

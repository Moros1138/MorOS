#ifndef MOROS_CONFIG_H
#define MOROS_CONFIG_H


// debug levels
//  0 = none (default)
//  1 = error
//  2 = info
#ifndef USE_MOROS_DEBUG
#define USE_MOROS_DEBUG 0
#endif

#if USE_MOROS_DEBUG > 0
#   if USE_MOROS_DEBUG >= 1
#       define MOROS_ERROR(...) printf(__VA_ARGS__)
#   else
#       define MOROS_ERROR(...) do { } while(false)
#   endif

#   if USE_MOROS_DEBUG >= 2
#       define MOROS_INFO(...) printf(__VA_ARGS__)
#   else
#       define MOROS_INFO(...) do { } while(false)
#   endif 
#else
#   define MOROS_ERROR(...) do { } while(false)
#   define MOROS_INFO(...) do { } while(false)
#endif

#if !defined(__MOROS_NAMESPACE__)
#   if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#       define __MOROS_NAMESPACE__ MorOS
#       include <stddef.h>
#   else
#       define __MOROS_NAMESPACE__ std
#       define __MOROS_KERNEL__
#   endif
#endif


#endif // MOROS_CONFIG_H

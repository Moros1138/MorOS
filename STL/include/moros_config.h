#define USE_MOROS_DEBUG

#ifdef USE_MOROS_DEBUG
#define MOROS_DEBUG(...) printf(__VA_ARGS__)
#else
#define MOROS_DEBUG(...) do { } while(false)
#endif

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#define __MOROS_NAMESPACE__ MorOS
#else
#define __MOROS_NAMESPACE__ std
#define __MOROS_KERNEL__
#endif


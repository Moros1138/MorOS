#define USE_MOROS_DEBUG

#ifdef USE_MOROS_DEBUG
#define MOROS_DEBUG(...) printf(__VA_ARGS__)
#else
#define MOROS_DEBUG(...) do { } while(false)
#endif


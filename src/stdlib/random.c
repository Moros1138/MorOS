#include "stdint.h"

// random numbers
static uint32_t rand_seed;

void srand(uint32_t seed)
{
    seed %= 0x7fffffff;
    seed = (seed > 0) ? seed : 12345;

    rand_seed = seed;
}

// https://en.wikipedia.org/wiki/Lehmer_random_number_generator#Sample_C99_code
int rand()
{
    rand_seed = (uint64_t)rand_seed * 48271 % 0x7fffffff;
    return (int)rand_seed;
}

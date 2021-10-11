#ifndef _MOROS_RANDOMIZER_H
#define _MOROS_RANDOMIZER_H

#include "types.h"

namespace MorOS
{

    class Random
    {
    public:
        Random();
        ~Random();

        static Random* activeRandomizer;

    public:
        void SetSeed(uint32_t s);
        int GetInteger();

    private:
        void Mutate();

    private:
        uint32_t seed;
    };

    void srand(uint32_t s);
    int rand();

} // MorOS


#endif // _MOROS_RANDOMIZER_H

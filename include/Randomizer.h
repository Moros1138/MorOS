#ifndef _MOROS_RANDOMIZER_H
#define _MOROS_RANDOMIZER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace MorOS
{

    class Randomizer
    {
    public:
        Randomizer();
        ~Randomizer();

        static Randomizer* activeRandomizer;

    public:
        void SetSeed(uint32_t s);
        int GetInteger();

    private:
        void Mutate();

    private:
        uint32_t seed;
    };

} // MorOS


#endif // _MOROS_RANDOMIZER_H

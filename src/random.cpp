#include "random.h"

namespace MorOS
{

	Random* Random::activeRandomizer = 0;

	Random::Random()
	{
		activeRandomizer = this;
		
		// set an initial non-zero seed
		seed = 12345;

		// That's AMAZING! I have the same
		// combination on my LUGGAGE!
	}

	Random::~Random()
	{

	}
	
	void Random::SetSeed(uint32_t s)
	{
		seed = s;
	}

	// https://en.wikipedia.org/wiki/Lehmer_random_number_generator#Schrage's_method
	void Random::Mutate()
	{
		// Precomputed parameters for Schrage's method
		const uint32_t M = 0x7fffffff;
		const uint32_t A = 48271;
		const uint32_t Q = M / A;    // 44488
		const uint32_t R = M % A;    //  3399

		uint32_t div = seed / Q;	// max: M / Q = A = 48,271
		uint32_t rem = seed % Q;	// max: Q - 1     = 44,487

		int32_t s = rem * A;	// max: 44,487 * 48,271 = 2,147,431,977 = 0x7fff3629
		int32_t t = div * R;	// max: 48,271 *  3,399 =   164,073,129
		int32_t result = s - t;

		if (result < 0)
			result += M;
		
		seed = result;
	}

	int Random::GetInteger()
	{
		Mutate();
		return (int)seed;
	}

    void srand(uint32_t s)
	{
		Random::activeRandomizer->SetSeed(s);
	}
    
	int rand()
	{
		return Random::activeRandomizer->GetInteger();
	}

} // MorOS


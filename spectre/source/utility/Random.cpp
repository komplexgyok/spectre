#include "Random.h"

#include <random>

namespace Spectre
{
	float Random::randomFloat(int min, int max)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<> distribution(min, max);

		return static_cast<float>(distribution(generator));
	}
}

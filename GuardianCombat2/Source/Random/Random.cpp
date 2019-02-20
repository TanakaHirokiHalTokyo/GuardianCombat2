#include "Random.h"

unsigned int GC2::Random::seed_ = std::random_device()();
std::minstd_rand GC2::Random::random_(seed_);

float GC2::Random::RangeFloat(float min,float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random_);
}

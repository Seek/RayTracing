#include "RNG.hpp"

RNG::RNG()
{
	std::random_device rd;
	engine_ptr = std::unique_ptr<mt_engine>(new mt_engine(rd()));
}

RNG::RNG(int seed)
{
	engine_ptr = std::unique_ptr<mt_engine>(new mt_engine(seed));
}

float RNG::nextFloat() const
{
	return uniform_float_dist(*engine_ptr);
}

int RNG::nextInt() const
{
	return uniform_int_dist(*engine_ptr);
}

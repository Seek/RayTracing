#ifndef RNG_HPP
#define RNG_HPP

#ifdef _MSC_VER
#pragma once
#endif //_MSC_VER

#include <memory>
#include <random>

class RNG {
public:
	RNG();
	RNG(int seed);
	float nextFloat() const;
	int nextInt() const;
private:
	std::uniform_real_distribution<float> uniform_float_dist{};
	std::uniform_int_distribution<int> uniform_int_dist{};
	typedef std::mt19937 mt_engine;
	std::unique_ptr<mt_engine> engine_ptr; 
};


#endif // !RNG_HPP

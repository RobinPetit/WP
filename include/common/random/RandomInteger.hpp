#ifndef _RANDOM_INTEGER_HPP_
#define _RANDOM_INTEGER_HPP_

#include <random>

/// RandomInteger is a class used to generate integers in a certain
/// range. It uses C++-11 <random> header. The reason it is here is to factorize the
/// random integer generation code
class RandomInteger
{
public:
	/// Constructor
	RandomInteger();

	/// generates an integer in [lowerBound, upperBound)
	/// \param upperBound The first integer that can not be generated
	/// \param lowerBound The first integer that can be generated
	/// \return A value in {lowerBound, lowerBound+1, ..., uppBound-2, upperBound-1}
	int next(int upperBound, int lowerBound=0);

private:

	std::random_device _device;   ///< Used to init the generator
	std::mt19937 _generator;   ///< used as seed in the generation
};

#endif  // _RANDOM_INTEGER_HPP_

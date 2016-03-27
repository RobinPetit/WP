#include "common/random/RandomInteger.hpp"

RandomInteger::RandomInteger():
	_device{},
	_generator{_device()}
{

}

int RandomInteger::next(int upperBound, int lowerBound)
{
	// as lowerBound is the first non possible number, remove 1 before to generate
	return std::uniform_int_distribution<int>(lowerBound, upperBound-1)(_generator);
}

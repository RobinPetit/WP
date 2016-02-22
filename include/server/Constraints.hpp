#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

#include <vector>
#include "server/CardData.hpp"


class Constraints
{
private:
	const std::pair<unsigned, CONSTRAINT_VALUE_RULE>* _defaultValues;
	const unsigned _size;
	std::vector<std::pair<unsigned, unsigned>>* _timedValues;

public:
	Constraints(const std::pair<unsigned, CONSTRAINT_VALUE_RULE>* defaultValues, const unsigned arraySize);
	void setConstraint(unsigned constraintID, unsigned value, unsigned turns);
	unsigned getConstraint(unsigned constraintID);
	void timeOutConstraints();
};

#endif  // _CONSTRAINTS_HPP_

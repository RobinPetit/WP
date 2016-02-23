#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

#include <vector>
#include "server/CardData.hpp"


class Constraints
{
private:
	const std::pair<int, CONSTRAINT_VALUE_RULE>* _defaultValues;
	const unsigned _size;
	std::vector<std::pair<int, int>>* _timedValues;

public:
	Constraints(const std::pair<int, CONSTRAINT_VALUE_RULE>* defaultValues, const int arraySize);
	void setConstraint(int constraintID, int value, int turns);
	int getConstraint(int constraintID);
	void timeOutConstraints();
};

#endif  // _CONSTRAINTS_HPP_

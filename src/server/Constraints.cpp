#include "server/Constraints.hpp"

ConstraintList::ConstraintList(const unsigned* defaultValues, const unsigned arraySize):
	_defaultValues(defaultValues), _size(arraySize)
{
	_timedValues = new std::vector<std::pair<unsigned, unsigned>>[_size];
}

void ConstraintList::setConstraint(unsigned constraintID, unsigned value, unsigned turns)
{
	_timedValues[constraintID].push_back(std::make_pair(value, turns));
}

unsigned ConstraintList::getConstraint(unsigned constraintID)
{
    if (_timedValues[constraintID].empty())
		return _defaultValues[constraintID];
    else
		return _timedValues[constraintID].rbegin()->first; //TODO change this model ?
}

void ConstraintList::timeOutConstraints()
{
    for (unsigned i=0; i<_size; i++)
    {
        std::vector<std::pair<unsigned, unsigned>> vect = _timedValues[i];
        for (std::vector<std::pair<unsigned, unsigned>>::iterator vectIt=vect.begin(); vectIt!=vect.end();)
        {
            if (vectIt->second == 1) vectIt = vect.erase(vectIt);
            else
            {
				vectIt->second--;
				vectIt++;
            }
        }
    }
}

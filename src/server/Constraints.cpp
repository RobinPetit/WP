#include "server/Constraints.hpp"

Constraints::Constraints(const std::pair<int, CONSTRAINT_VALUE_RULE>* defaultValues, const int arraySize):
	_defaultValues(defaultValues), _size(arraySize)
{
	_timedValues = new std::vector<std::pair<int, int>>[_size];
}

void Constraints::setConstraint(int constraintID, int value, int turns)
{
	_timedValues[constraintID].push_back(std::make_pair(value, turns));
}

int Constraints::getConstraint(int constraintID)
{
    if (_timedValues[constraintID].empty())
		return _defaultValues[constraintID].first;
    else
    {
		int value = _timedValues[constraintID].rbegin()->first;
		switch(_defaultValues[constraintID].second) //rules
		{
			case VALUE_GET_INCREMENT:
                _timedValues[constraintID].rbegin()->first++;
                break;
			case VALUE_GET_DECREMENT:
				_timedValues[constraintID].rbegin()->first--;
				break;
			default:
				//no nothing to value
				break;
		}
		return value;
    }
}

void Constraints::timeOutConstraints()
{
    for (int i=0; i<_size; i++)
    {
        std::vector<std::pair<int, int>> vect = _timedValues[i];
        for (std::vector<std::pair<int, int>>::iterator vectIt=vect.begin(); vectIt!=vect.end();)
        {
			//TODO how do I delete value without breaking iterator ?
            if (vectIt->second == 1) vectIt = vect.erase(vectIt);
            else
            {
				vectIt->second--;
                switch (_defaultValues[i].second) //rules
                {
					case VALUE_TURN_INCREMENT:
						vectIt->first++;
						break;
					case VALUE_TURN_DECREMENT:
						vectIt->first--;
						break;
					default:
						//do nothing to value
						break;
                }
				vectIt++;
            }
        }
    }
}

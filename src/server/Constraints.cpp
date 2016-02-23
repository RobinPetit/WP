#include "server/Constraints.hpp"

Constraints::Constraints(const ConstraintDefaultValue* defaultValues, const int arraySize):
	_size(arraySize), _defaultValues(defaultValues)
{
	_timedValues = new std::vector<ConstraintTimedValue>[_size];
}

void Constraints::setConstraint(int constraintID, int value, int turns)
{
	_timedValues[constraintID].push_back({value, turns, nullptr});
}

void Constraints::setConstraint(int constraintID, int value, int turns, const Creature* caster)
{
    _timedValues[constraintID].push_back({value, turns, caster});
}

int Constraints::getConstraint(int constraintID)
{
	int value;

	if (_timedValues[constraintID].empty())
		value = _defaultValues[constraintID].value;
    else
    {
		switch (_defaultValues[constraintID].getOption)
		{
			case GET_FIRST:
				value = getTimedValue(constraintID, _timedValues[constraintID].size()-1);
            case GET_LAST:
				value = getTimedValue(constraintID, 0);
				break;
            case GET_SUM:
				for (unsigned i=0; i<_timedValues[constraintID].size(); i++)
				{
                    value += getTimedValue(constraintID, i);
				}

		}
	}
	return value;
}

int Constraints::getTimedValue(int constraintID, unsigned valueIndex)
{
    int value = _timedValues[constraintID].at(valueIndex).value;
    switch(_defaultValues[constraintID].valueOption) //rules
	{
		case VALUE_GET_INCREMENT:
			_timedValues[constraintID].at(valueIndex).value++;
			break;
		case VALUE_GET_DECREMENT:
			_timedValues[constraintID].at(valueIndex).value--;
			break;
		default:
			// no nothing to value
			break;
	}
	return value;
}

void Constraints::timeOutConstraints()
{
	for (unsigned i=0; i<_size; i++)
	{
		std::vector<ConstraintTimedValue>& vect = _timedValues[i]; //value, time left
		for (auto vectIt=vect.begin(); vectIt!=vect.end();)
		{
			// TODO how do I delete value without breaking iterator ?
			if (vectIt->turns == 1)
				vectIt = vect.erase(vectIt);
			else
			{
				vectIt->turns--;
				switch (_defaultValues[i].valueOption)  // rules
				{
					case VALUE_TURN_INCREMENT:
						vectIt->value++;
						break;
					case VALUE_TURN_DECREMENT:
						vectIt->value--;
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

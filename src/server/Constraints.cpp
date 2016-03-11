#include "server/Constraints.hpp"
#include "server/Creature.hpp"

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

int Constraints::getConstraint(int constraintID) const
{
	switch (_defaultValues[constraintID].orderOption)
	{
		case GET_FIRST:
			return getFirstTimedValue(constraintID);
		case GET_LAST:
			return getLastTimedValue(constraintID);
		case GET_SUM:
			return getSumTimedValues(constraintID);
	}
	throw std::runtime_error("Order option not valid");
}

int Constraints::getOverallConstraint(int constraintID, int otherValue) const
{
	switch (_defaultValues[constraintID].orderOption)
	{
		case GET_FIRST:
			{
				if (otherValue == _defaultValues[constraintID].value)
					return getFirstTimedValue(constraintID);
				else
					return otherValue;
			}
			break;
		case GET_LAST:
			{
				if (otherValue == _defaultValues[constraintID].value)
					return getLastTimedValue(constraintID);
				else
					return otherValue;
			}
			break;
		case GET_SUM:
			{
				otherValue += getSumTimedValues(constraintID);
				return otherValue;
			}
			break;
	}
	throw std::runtime_error("Order option not valid");
}

int Constraints::getValue(int constraintID, unsigned valueIndex) const
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

int Constraints::getFirstTimedValue(int constraintID) const
{
	std::vector<ConstraintTimedValue>& vect = _timedValues[constraintID]; //value, turns left, caster
	for (auto vectIt=vect.begin(); vectIt!=vect.end();)
	{
		//if the caster is not remembered, or is on the board
		if (vectIt->caster==nullptr or vectIt->caster->isOnBoard() or vectIt->caster->getConstraint(CC_TEMP_IS_PARALYZED)==0)
			return getValue(constraintID, static_cast<unsigned>(vectIt - vect.begin()));

		//if caster is dead or paralyzed
		else
			vectIt++;
	}
	return _defaultValues[constraintID].value;
}

int Constraints::getLastTimedValue(int constraintID) const
{
	std::vector<ConstraintTimedValue>& vect = _timedValues[constraintID]; //value, turns left, caster
	for (auto vectIt=vect.rbegin(); vectIt!=vect.rend();)
	{
		//if the caster is not remembered, or is on the board and active
		if (vectIt->caster==nullptr or vectIt->caster->isOnBoard() or vectIt->caster->getConstraint(CC_TEMP_IS_PARALYZED)==0)
			return getValue(constraintID, static_cast<unsigned>(vectIt - vect.rbegin()));

		//if caster is dead or paralyzed
		else
			vectIt++;
	}
	return _defaultValues[constraintID].value;
}

int Constraints::getSumTimedValues(int constraintID) const
{
	int value = _defaultValues[constraintID].value;
	std::vector<ConstraintTimedValue>& vect = _timedValues[constraintID]; //value, turns left, caster
	for (auto vectIt=vect.begin(); vectIt!=vect.end();)
	{
		//if the caster is not remembered, or is on the board and active
		if (vectIt->caster==nullptr or vectIt->caster->isOnBoard() or vectIt->caster->getConstraint(CC_TEMP_IS_PARALYZED)==0)
			value += getValue(constraintID, static_cast<unsigned>(vectIt - vect.begin()));

		//if caster is dead or paralyzed
		else
			vectIt++;
	}
	return value;
}

void Constraints::timeOutConstraints()
{
	for (unsigned i=0; i<_size; i++)
	{
		std::vector<ConstraintTimedValue>& vect = _timedValues[i]; //value, turns left, caster
		for (auto vectIt=vect.begin(); vectIt!=vect.end();)
		{
			//if the constraint has run our of turns or if its caster has died
			if (vectIt->turns==1 or not(vectIt->caster==nullptr or vectIt->caster->isOnBoard()))
				vectIt = vect.erase(vectIt); //returns iterator to following object
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

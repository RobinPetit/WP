#ifndef _CARD_SERVER_HPP
#define _CARD_SERVER_HPP

#include <string>
#include <vector>
#include <iostream>


class Card
{
private:
	unsigned _cost;
	std::vector<std::vector<unsigned>> _effects;	//Effects

public:
	/// Constructor
	Card(unsigned cost=0, std::vector<std::vector<unsigned>> effects = {{}});

	/// Getters
	unsigned getCost(){return _cost;}
	std::vector<std::vector<unsigned>> getEffects();


	/// Methods
	virtual bool isCreature()=0;
	virtual bool isSpell()=0;

	virtual ~Card()=default;

};

#endif //_CARD_SERVER_HPP

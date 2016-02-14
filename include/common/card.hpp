#ifndef _CARD_COMMON_HPP
#define _CARD_COMMON_HPP

#include <string>
#include "effect.hpp"

class Card{
    unsigned int _cost;
    std::string _name;
    Effect _effect;
    
public:

    ///Constructors
    Card (unsigned int cost, std::string name, Effect effect):
                            _cost(cost), _name(name), _effect(effect){};
    
    ///Getters
    unsigned int getCost(){return _cost;};
    Effect getEffect(){return _effect;};
    std::string getName(){return _name;};

};

#endif //_CARD_COMMON_HPP

#ifndef _CARD_COMMON_HPP
#define _CARD_COMMON_HPP

#include <string>
#include "effect.hpp"

template <typename Param>
class Card{
    unsigned int _cost;
    Effect<Param> _effect;
    std::string _name;
    
public:

    ///Constructors
    Card (unsigned int cost, Effect<Param> effect, std::string name):
                            _cost(cost), _effect(effect), _name(name){};
    
    ///Getters
    unsigned int getCost(){return _cost;};
    Effect<Param> getEffect(){return _effect;};
    std::string getName(){return _name;};

};

#endif //_CARD_COMMON_HPP

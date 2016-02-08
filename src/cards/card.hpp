#ifndef _CARD_COMMON_HPP
#define _CARD_COMMON_HPP

#include "effect.hpp"

template <typename Param>
class Card{
    unsigned int _cost;
    Effect<Param> _effect;
    char* _img;
    
public:

    ///Constructors
    Card (unsigned int cost, Effect<Param> effect, char* img):
                            _cost(cost), _effect(effect), _img(img){};
    
    ///Getters
    unsigned int getCost(){return _cost;};
    Effect<Param> getEffect(){return _effect;};
    char* getImg(){return _img;};

};

#endif //_CARD_COMMON_HPP

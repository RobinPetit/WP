#ifndef _CARD_COMMON_HPP
#define _CARD_COMMON_HPP

#include <string>
#include <vector>
#include <iostream>

class Card
{
    unsigned int _cost;
    std::string _name;
    std::vector<int> _effect;    //~ Create an empty Vector which will contain all the required variables for his effect.

public:

    ///Constructors
    Card(unsigned int cost = 0, std::string name = "No name" , std::vector<int> effect = {0}):
            _cost(cost), _name(name), _effect(effect){};

    ///Getters
    unsigned int getCost(){return _cost;}
    int getEffect(){return _effect[0];}
    std::string getName(){return _name;}

    ///Methodes
    virtual void print() {std::cout << _name;}
    
    virtual ~Card()=default;

};

#endif //_CARD_COMMON_HPP

#ifndef _CARD_SERVER_HPP
#define _CARD_SERVER_HPP

#include <string>
#include <vector>
#include <iostream>


class Card
{
    unsigned int _cost;
    std::string _name;
    std::vector<std::vector<unsigned>> _effects;    //~ Create an empty Vector which will contain all the effects and their variables.

public:

    ///Constructors
    Card(unsigned int cost = 0, std::string name = "No name" , std::vector<std::vector<unsigned>> effect = {{}}):
            _cost(cost), _name(name), _effects(effect){};

    ///Getters
    unsigned int getCost(){return _cost;}
    std::vector<std::vector<unsigned>> getEffects() {return _effects;}
    std::string getName(){return _name;}

    ///Methodes
    virtual void print() {std::cout << _name;}
    virtual bool isCreature()=0;
    virtual bool isSpell()=0;
    virtual bool hasEffects() { return not _effects.empty(); };

    virtual ~Card()=default;

};

#endif //_CARD_SERVER_HPP

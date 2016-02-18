#ifndef _CARD_SERVER_HPP
#define _CARD_SERVER_HPP

#include <string>
#include <vector>
#include <iostream>


class Card
{
private:
    unsigned _cost;
    std::string _name;
    std::vector<std::vector<unsigned>> _instantEffects;    //~ One type of effects

public:

    ///Constructors
    Card(unsigned cost = 0, std::string name = "No name" , std::vector<std::vector<unsigned>> effect = {{}});

    ///Getters
    unsigned getCost(){return _cost;}
    std::vector<std::vector<unsigned>> getInstantEffects();
    //inline std::string getName(){return _name;}


    ///Methodes
    //virtual void print() {std::cout << _name;}
    virtual bool isCreature()=0;
    virtual bool isSpell()=0;

    virtual ~Card()=default;

};

#endif //_CARD_SERVER_HPP

#ifndef _GAME_STATE_CLIENT_HPP
#define _GAME_STATE_CLIENT_HPP

#define DECK_SIZE = 20;
#define DFLT_ENERG = 0;

#include <vector>
#include "client/AbstractState.hpp"
//~I'm not sure about that. Is it right to import something from the server ? (NBA)
#include "server/Card.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class GameState : public AbstractState
{
	public:
		/// Constructor.
		GameState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;
		
		//~Didn't catch the difference between begin and start turn (NBA).
		void begin(unsigned lotsOfDataAboutStuff);
		void startTurn(unsigned);
        
        void changeEnergy(unsigned);  ///Can be used for effects

	private:
		std::vector<Card> _inHand;
		std::vector<Card> _onBoard;
		unsigned _remainCards = DECK_SIZE;
		unsigned _energy = DEFLT_ENERG;
		unsigned _oppoCards;
		bool _myTurn=false;
        
        void setEnergy(unsigned);
		void useCard();
		void attackWithCreature();
		void endTurn();
		void quit();

};

GameState::GameState(StateStack& stateStack, Client& client)
{
}

void GameState::startTurn(unsigned turnNbr)
{
	_myTurn = true
	//TODO put the card taken in the _inHand vector;
	--_remainCards; ///Player took a card from his deck
	this.setEnergy(turnNbr);		
}

void GameState::changeEnergy(unsigned energy){
    if(_energy+energy >= 10){
        _energy = 10;}
    else
        _energy+=energy;
}

void GameState::setEnergy(unsigned energy)
{
	if(energy >= 10){
		///Energy can't be higher than 10
		_energy = 10;}
	else
		_energy = energy;
}

void GameState::endTurn()
{
    this.setEnergy(DFLT_ENERG);
    _myTurn = false;
}
	
#endif// _GAME_STATE_CLIENT_HPP


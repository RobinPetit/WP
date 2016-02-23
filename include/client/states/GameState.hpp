#ifndef _GAME_STATE_CLIENT_HPP
#define _GAME_STATE_CLIENT_HPP

constexpr unsigned DECK_SIZE = 20;
constexpr unsigned DFLT_ENERGY = 0;
constexpr unsigned MAX_ENERGY = 10;

#include <vector>
#include "client/AbstractState.hpp"

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

		void begin(unsigned lotsOfDataAboutStuff);
		void startTurn();

		void changeEnergy(unsigned);  ///Can be used for effects

	private:
		std::vector<int> _inHand;
		std::vector<int> _onBoard;
        std::vector<int> _oppoBoard;
		unsigned _lotsOfDataAboutStuff;
		unsigned _remainCards = DECK_SIZE;
		unsigned _energy = DFLT_ENERGY;
		unsigned _oppoCards;
		unsigned _nbrTurn = 0;
		bool _myTurn;

		void setEnergy(unsigned);
		void useCard();
		void putOnBoard(std::size_t);
		void attackWithCreature();
		void endTurn();
		void quit();

};

#endif  // _GAME_STATE_CLIENT_HPP


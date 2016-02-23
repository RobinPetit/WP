#ifndef _GAME_STATE_CLIENT_HPP
#define _GAME_STATE_CLIENT_HPP

constexpr unsigned DECK_SIZE = 20;
constexpr unsigned DFLT_ENERG = 0;
constexpr unsigned MAX_ENERG = 10;

#include <vector>
#include "client/AbstractState.hpp"
// \TODO place Card in common or create a new class for client
// No it's wrong
// #include "server/Card.hpp"

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
		void startTurn();

		void changeEnergy(unsigned);  ///Can be used for effects

	private:
		// Commented because Card does not exist yet
		// std::vector<Card> _inHand;
		// std::vector<Card> _onBoard;
		unsigned _lotsOfDataAboutStuff;
		unsigned _remainCards = DECK_SIZE;
		unsigned _energy = DFLT_ENERG;
		unsigned _oppoCards;
		bool _myTurn;

		void setEnergy(unsigned);
		void useCard();
		void attackWithCreature();
		void endTurn();
		void quit();

};

#endif// _GAME_STATE_CLIENT_HPP


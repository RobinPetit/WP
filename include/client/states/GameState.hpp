#ifndef _GAME_STATE_CLIENT_HPP
#define _GAME_STATE_CLIENT_HPP

// std-C++ headers
#include <vector>
#include <array>
#include <atomic>
// WizardPoker headers
#include "client/AbstractState.hpp"
#include "client/NonBlockingInput.hpp"

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

		void begin();
		void play();
		void startTurn();
		void updateData(std::array<unsigned, 5>);
		void pickCard(int);

		///Those methods can be used for effects
		void changeHealth(int);
		void changeOppoHealth(int);
		void changeEnergy(int);

	private:
		std::vector<int> _inHand;
		std::vector<int> _onBoard;
		std::vector<int> _oppoBoard;
		unsigned _remainCards;
		unsigned _selfHealth;
		unsigned _oppoHealth;
		unsigned _energy;
		unsigned _oppoCards;
		// \TODO: sync with Client::_inGame
		std::atomic_bool _playing;
		std::atomic_bool _myTurn;
		NonBlockingInput _nonBlockingInput;
		/// Waits for special server data such as END_OF_TURN, BOARD_UPDATE, etc.
		std::thread _listeningThread;

		std::size_t selectHand();
		std::size_t selectBoard();
		std::size_t selectOppo();

		void useCard();
		void putOnBoard(std::size_t);
		void attackWithCreature();
		void applyOppoEffect();
		void applySelfEffect();
		void endTurn();
		void quit();

		/// Start the new thread waiting for special data
		void initListening();
		/// Called by the game listening thread: waits for server game thread special data
		void inputListening();
};

#endif  // _GAME_STATE_CLIENT_HPP


#ifndef _GAME_STATE_CLIENT_HPP
#define _GAME_STATE_CLIENT_HPP

// std-C++ headers
#include <vector>
#include <array>
#include <atomic>
#include <mutex>
// WizardPoker headers
#include "client/AbstractState.hpp"
#include "client/NonBlockingInput.hpp"
#include "common/CardData.hpp"

// Forward declarations
class StateStack;

/// General card data
struct CardData
{
	std::string name;
	CostValue cost;
	std::string description;
};

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

		~GameState();

	private:
		///////////////// attributes

		std::vector<sf::Uint32> _selfHandCards;
		std::vector<sf::Uint32> _selfBoardCards;
		std::vector<sf::Uint32> _selfGraveCards; //needed for reviving spells
		std::vector<sf::Uint32> _oppoBoardCards;
		unsigned _selfDeckSize;
		unsigned _oppoHandSize;


		unsigned _selfHealth, _oppoHealth;
		unsigned _selfEnergy;

		// allow to update these values without interfering during a turn
		std::mutex _accessHealth;
		std::mutex _accessEnergy;
		// \TODO: sync with Client::_inGame
		std::atomic_bool _playing;
		std::atomic_bool _myTurn;
		NonBlockingInput _nonBlockingInput;
		/// Waits for special server data such as END_OF_TURN, BOARD_UPDATE, etc.
		std::thread _listeningThread;

		//////////////// private methods

		// called by the constructor to init the object
		void init();
		void chooseDeck();

		// Requests user for additional input
		int askIndex(int maxIndex, std::string inputMessage);
		int askSelfHandIndex();
		int askSelfBoardIndex();
		int askSelfGraveyardIndex();
		int askOppoBoardIndex();
		int askOppoHandIndex();

		// User interface ("actions")
		void useCard();
		void attackWithCreature();
		void endTurn();
		void quit();

		// Game display
		void displayGame();
		void displayCardVector(std::vector<sf::Uint32> cardVector);
		void displayBoardVector(std::vector<sf::Uint32> cardVector);
		CardData getCardData(int cardID);

		/// Start the new thread waiting for special data
		void initListening();
		/// Called by the game listening thread: waits for server game thread special data
		void inputListening();

		// Not needed ?
		///Those methods can be used for effects
		//void changeHealth(int);
		//void changeOppoHealth(int);
		//void changeEnergy(int);
		//void pickCard(int); //HAPPENS IN SERVER
		//void applyOppoEffect();
		//void applySelfEffect();
		//void putOnBoard(std::size_t);
};

#endif  // _GAME_STATE_CLIENT_HPP


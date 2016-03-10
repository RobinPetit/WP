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
#include "common/GameData.hpp"
#include "common/Identifiers.hpp"
#include "common/sockets/TransferType.hpp"

/// The class representing the gameplay for the client
class Game
{
	public:
		/// Constructor.
		Game(Client& client);

		void play();
		void startTurn();

		~Game();

	private:
		///////////////// attributes

		Client& _client;

		std::vector<CardData> _selfHandCards;
		std::vector<CardData> _selfGraveCards;  // Needed for reviving spells

		std::vector<BoardCreatureData> _selfBoardCreatures;
		std::vector<BoardCreatureData> _oppoBoardCreatures;

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
		/// Waits for special server data such as END_OF_TURN, BOARD_UPDATE, etc.
		std::thread _listeningThread;

		//////////////// private methods

		// called by the constructor to init the object
		void init();
		void chooseDeck();

		// Requests user for additional input
		int askIndex(std::size_t upperBound, const std::string& inputMessage);
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
		void endGame(sf::Packet& transmission);  // only used to lighten handlePacket

		// Game display
		void display();
		void displayGame();
		void displayCardVector(const std::vector<CardData>& cardVector, bool displayDescription=false);
		void displayBoardCreatureVector(const std::vector<BoardCreatureData>& cardVector, bool displayDescription=false);

		const std::string& getCardName(cardId id);
		CostValue getCardCost(cardId id);
		const std::string& getCardDescription(cardId id);

		/// Start the new thread waiting for special data
		void initListening();
		/// Called by the game listening thread: waits for server game thread special data
		void inputListening();

		/// Handles the whole transmission until transmission.endOfPacket()
		void handlePacket(sf::Packet& transmission);

		/// Those methods can be used for effects
		//void pickCard(int); //HAPPENS IN SERVER
		//void applyOppoEffect();
		//void applySelfEffect();
		//void putOnBoard(std::size_t);

		//////////// static member

		static const std::vector<std::pair<const std::string, void (Game::*)()>> _actions;

		template <typename FixedSizeIntegerType>
		static FixedSizeIntegerType receiveFromPacket(sf::Packet& receivedPacket);
};

#endif  // _GAME_STATE_CLIENT_HPP

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
class AbstractGame
{
	public:
		/// Constructor.
		AbstractGame(Client& client);

		void init();
		void play();
		void startTurn();

		virtual ~AbstractGame();

	protected:
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

		//////////// static members

		static const std::vector<std::pair<const std::string, void (AbstractGame::*)()>> _actions;

		//////////// protected methods

		const std::string& getCardName(cardId id);
		CostValue getCardCost(cardId id);
		const std::string& getCardDescription(cardId id);

	private:
		//////////////// private methods

		// called by the constructor to init the object
		virtual void chooseDeck() = 0;

		// User interface ("actions")
		void useCard();
		void attackWithCreature();
		void endTurn();
		void quit();
		void endGame(sf::Packet& transmission);  // only used to lighten handlePacket

		// Game display
		virtual void display() = 0;
		virtual void displayGame() = 0;

		virtual void displayMessage(const std::string& message) = 0;

		// game "inputs"
		virtual int askSelfHandIndex() = 0;
		virtual int askSelfBoardIndex() = 0;
		virtual int askSelfGraveyardIndex() = 0;
		virtual int askOppoBoardIndex() = 0;
		virtual int askOppoHandIndex() = 0;

		/// Start the new thread waiting for special data
		void initListening();
		/// Called by the game listening thread: waits for server game thread special data
		void inputListening();

		/// Handles the whole transmission until transmission.endOfPacket()
		void handlePacket(sf::Packet& transmission);

		virtual void receiveCard(cardId id) = 0;

		//////////// static member

		template <typename FixedSizeIntegerType>
		static FixedSizeIntegerType receiveFromPacket(sf::Packet& receivedPacket);
};

#endif  // _GAME_STATE_CLIENT_HPP

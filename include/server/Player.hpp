#ifndef _PLAYER_HPP
#define _PLAYER_HPP

// std-C++ headers
#include <stack>
#include <random>
#include <array>
#include <functional>
#include <cstddef>
#include <atomic>
#include <memory>
// WizardPoker headers
#include "server/Card.hpp"
#include "server/Spell.hpp"
#include "server/Creature.hpp"
#include "server/Constraints.hpp"
#include "server/ServerDatabase.hpp"
#include "common/CardData.hpp"
#include "common/GameData.hpp"
#include "common/Identifiers.hpp"  // userId
#include "common/sockets/TransferType.hpp"
#include "common/sockets/EndGame.hpp"
#include "common/Deck.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class GameThread;

/// Represents one of the two players for a given game.
class Player
{
public:
	/*------------------------------ Methods */
	/// Constructor
	Player(GameThread& gameThread, ServerDatabase& database, userId id, Player& opponent);

	// Interface for basic gameplay
	/// Receive the deck sent by the client and put it in the game
	void receiveDeck();

	/// The game has begun.
	void setUpGame(bool isActivePlayer);

	/// The player's turn has started
	void enterTurn(int turn);

	/// The player's turn has ended
	void leaveTurn();

	// Interface for client input
	/// Tries to receive an input from the client, executes the corresponding
	/// action.
	/// \return the status of the socket after the receiving
	sf::Socket::Status tryReceiveClientInput();

	/// \return true if some changes has been logged since the last player's
	/// action, false otherwise.
	bool thereAreBoardChanges();

	/// This method clears the pending board changes and returns them.
	/// \return the changes that occured on the board if thereAreBoardChanges(),
	/// an empty sf::Packet otherwise.
	/// \post !thereAreBoardChanges();
	sf::Packet getBoardChanges();

	/// This method is called by a creature when it dies to be remvoed from
	/// the board and to be placed in the graveyard
	void cardBoardToGraveyard(const Creature *card);

	// Getters
	int getCreatureConstraint(const Creature& subject, int constraintId) const;
	const Card* getLastCaster() const;
	userId getId() const;
	sf::TcpSocket& getSocket();
	void printVerbose(std::string message);

private:
	/*------------------------------ Types */
	struct TurnData
	{
		int turnCount;
		int cardsUsed;
		int creaturesPlaced;
		int creatureAttacks;
		int spellCalls;
	};

	/*------------------------------ Static variables */
	static const int _maxEnergy = 10, _maxHealth = 20;
	constexpr static TurnData _emptyTurnData = {0, 0, 0, 0, 0};
	constexpr static unsigned _initialSupplementOfCards{4};
	constexpr static unsigned _maximumAmountOfTurnsWithEmptyDeck{10};

	/*------------------------------ Attributes */
	GameThread& _gameThread;
	ServerDatabase& _database;

	Player& _opponent;
	userId _id;
	std::atomic_bool _isActive; // blocks functions that are only allowed for active player

	// Client communication
	sf::TcpSocket _socketToClient;
	sf::Packet _pendingBoardChanges;

	// Gameplay
	int _energyInit, _energy, _healthInit, _health;
	unsigned _turnsSinceEmptyDeck;
	TurnData _turnData;

	// Constraints
	Constraints _constraints = Constraints(playerDefaultConstraints);
	Constraints _teamConstraints = Constraints(creatureDefaultConstraints);

	// Card holders
	// The sum of the lengths of these std::vectors/std::stacks is **always** 20
	// because at first, all are empty except the deck which contains... The deck
	// obviously... And then the cards move from one to another but never disappear
	// or are created

	/// Cards that are in the deck (not usable yet)
	std::stack<std::unique_ptr<Card>> _cardDeck;

	/// Cards that are in the player's hand (usable)
	std::vector<std::unique_ptr<Card>> _cardHand;

	/// Cards that are on the board (usable for attacks)
	std::vector<std::unique_ptr<Creature>> _cardBoard;

	/// Cards that are discarded (dead creatures, used spells)
	std::vector<std::unique_ptr<Card>> _cardGraveyard;

	/// Last card that was used to cast an effect (his or his opponent's)
	/// This is not a smart pointer because it points to an already allocated card.
	const Card* _lastCasterCard = nullptr;

	// Effects container
	static std::array<std::function<void(Player&, EffectArgs)>, P_EFFECTS_COUNT> _effectMethods;

	/*------------------------------ Methods */
	// User actions
	/// Use a card
	void useCard(int handIndex);

	/// Attack victim (-1 for opponent) with a card
	void attackWithCreature(int boardIndex, int victim);

	/// End the turn
	void endTurn();

	/// The game has ended because of some reason (maybe because the user want to quit the game)
	void finishGame(bool hasWon, EndGame::Cause cause);

	// Interface for applying effects
	/// Generic method that will then call the appropriate method below.
	/// \return True if the effect could have been applied and false otherwise
	bool applyEffect(Card* usedCard, EffectArgs effect);

	/// Apply an effect to itself
	void applyEffectToSelf(EffectArgs effect);

	/// Apply an effect to one of its Creatures, with reference to creature
	void applyEffectToCreature(Creature* casterAndSubject, EffectArgs effect);

	/// Apply an effect to one of its Creatures, with creature index
	void applyEffectToCreature(EffectArgs effect, const std::vector<int>& boardIndexes);

	/// Apply an effect to all of its Creatures
	void applyEffectToCreatureTeam(EffectArgs effect);

	/// Generate a ramdom number in the interval [0, vector.size()[. This is a
	/// convenience function used only to lightweight the code.
	/// \tparam Type contained in the vector.
	/// \param vector The vector that need a random index.
	/// \return A ramdom number in the interval [0, vector.size()[.
	/// \note The implementation is in source file rather than in the header
	/// because this is a private method.
	template <typename T>
	inline int getRandomIndex(const std::vector<T>& vector);

	/// \param selection a vector of values telling whether the choice must be in player's cards or opponent's cards
	/// \return a vector of indices selected
	std::vector<int> getRandomBoardIndexes(const std::vector<CardToSelect>& selection);

	/// \param selection a vector of values telling whether the choice must be in player's cards or opponent's cards
	/// \return a vector of indices selected
	std::vector<int> askUserToSelectCards(const std::vector<CardToSelect>& selection);

	// Effects (private)
	void setConstraint(EffectArgs effect);
	void pickDeckCards(EffectArgs effect);
	void loseHandCards(EffectArgs effect);
	void reviveGraveyardCard(EffectArgs effect);
	void stealHandCard(EffectArgs effect);
	void exchgHandCard(EffectArgs effect);
	void resetEnergy(EffectArgs effect);
	void changeEnergy(EffectArgs effect);
	void changeHealth(EffectArgs effect);

	// Other private methods
	/// The method calling all of the effects generated by the presence
	/// of the card \a usedCard
	/// \param usedCard The card to exploit
	/// \return True if all of the effects could have been applied
	/// and false otherwise
	bool exploitCardEffects(Card* usedCard);
	void setTeamConstraint(EffectArgs effect);
	void setDeck(const Deck& newDeck);

	void cardDeckToHand(int amount);
	void cardHandToBoard(int handIndex);

	/// Move the card at handIndex from the player's hand to the bin
	void cardHandToGraveyard(int handIndex);

	/// Move the card at boardIndex from the board to the bin
	void cardBoardToGraveyard(int boardIndex);
	void cardGraveyardToHand(int binIndex);
	void cardAddToHand(std::unique_ptr<Card> given);
	std::unique_ptr<Card> cardRemoveFromHand();
	std::unique_ptr<Card> cardExchangeFromHand(std::unique_ptr<Card> given);
	std::unique_ptr<Card> cardExchangeFromHand(std::unique_ptr<Card> given, int handIndex);

	void useCreature(int handIndex, Card* usedCard);
	void useSpell(int handIndex, Card* useSpell);

	void logEverything();

	void logCurrentEnergy();
	void logCurrentHealth();
	void logOpponentHealth();
	void logCurrentDeck();

	void logHandState();
	void logOpponentHandState();
	void logBoardState();
	void logOpponentBoardState();
	void logGraveyardState();


	template <typename CardType>
	void logIdsFromVector(TransferType type, const std::vector<std::unique_ptr<CardType>>& vect);
	void logCardDataFromVector(TransferType type, const std::vector<std::unique_ptr<Card>>& vect);
	void logBoardCreatureDataFromVector(TransferType type, const std::vector<std::unique_ptr<Creature>>& vect);
	void sendValueToClient(TransferType value);

	// Some getters
	const std::vector<std::unique_ptr<Creature>>& getBoard() const;
	int getHealth() const;
	std::vector<std::unique_ptr<Card>>::size_type getHandSize() const;
};


#endif  // _PLAYER_HPP

#ifndef _PLAYER_HPP
#define _PLAYER_HPP

// std-C++ headers
#include <stack>
#include <random>
#include <array>
#include <functional>
#include <cstddef>
#include <atomic>
// WizardPoker headers
#include "common/Card.hpp"
#include "server/Spell.hpp"
#include "server/Constraints.hpp"
#include "server/ServerDatabase.hpp"
#include "server/ServerCardData.hpp"
#include "common/GameData.hpp"
#include "common/Identifiers.hpp"  // userId
#include "common/sockets/TransferType.hpp"
#include "common/sockets/EndGame.hpp"
#include "common/Deck.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class GameThread;
class Creature;

/// Represents one of the two players for a given game.
class Player
{
public:
	/// Constructor
	Player(GameThread& gameThread, ServerDatabase& database, userId id);
	void setOpponent(Player* opponent);  // Complementary

	/// Destructor.
	~Player();

	// Interface for basic gameplay
	/// Receive the deck sent by the client and put it in the game
	void receiveDeck();
	/// The game has begun.
	void beginGame(bool isActivePlayer);
	/// The player's turn has started
	void enterTurn(int turn);
	/// The player's turn has ended
	void leaveTurn();

	// Interface for client input

	/// Tries to receive an input from the client, executes the corresponding
	/// action.
	/// \return the status of the socket after the receiving
	sf::Socket::Status tryReceiveClientInput();

	/// Interface for applying effects
	void applyEffect(Card* usedCard, EffectParamsCollection effect);
	//to itself
	void applyEffectToSelf(const Card* usedCard, EffectParamsCollection& effectArgs);
	//to one of its Creatures
	void applyEffectToCreature(Creature* casterAndSubject, EffectParamsCollection& effectArgs); //With ref. to creature
	void applyEffectToCreature(const Card* usedCard, EffectParamsCollection& effectArgs, const std::vector<int>& boardIndexes); //With creature index
	//to all of its Creatures
	void applyEffectToCreatureTeam(const Card* usedCard, EffectParamsCollection& effectArgs);

	/// Getters
	int getCreatureConstraint(const Creature& subject, int constraintID) const;
	const Card* getLastCaster() const;
	userId getID() const;
	sf::TcpSocket& getSocket();
	const std::vector<Creature *>& getBoard() const;
	int getHealth() const;
	std::vector<Card *>::size_type getHandSize() const;

	/// \return true if some changes has been logged since the last player's
	/// action, false otherwise.
	bool thereAreBoardChanges();

	/// This method clear the pending board changes and return them.
	/// \return the changes that occured on the board if therAreBoardChanges(),
	/// an empty sf::Packet otherwise.
	/// \post !thereAreBoardChanges();
	sf::Packet getBoardChanges();

	/// \return a vector of indices selected
	/// \param selection a vector of values telling whether the choice must be in player's cards or opponent's cards
	std::vector<int>&& getRandomBoardIndexes(const std::vector<CardToSelect>& selection);
	std::vector<int>&& askUserToSelectCards(const std::vector<CardToSelect>& selection);

private:
	/// Types
	struct TurnData
	{
		int turnCount;
		int cardsUsed;
		int creaturesPlaced;
		int creatureAttacks;
		int spellCalls;
	};

	/// const{expr} static variables
	constexpr static TurnData _emptyTurnData = {0, 0, 0, 0, 0};
	constexpr static unsigned _initialAmountOfCards{4};

	/// Attributes
	GameThread& _gameThread;
	ServerDatabase& _database;
	// Pointer responsability is not given to this Player: it is not an allocated-attribute
	Player* _opponent = nullptr;
	userId _id;
	std::atomic_bool _isActive;

	//Client communication
	sf::TcpSocket _socketToClient;
	sf::Packet _pendingBoardChanges;

	// Gameplay
	int _energy = 1, _energyInit = 1, _health = 20, _healthInit = 20;
	int _turnsSinceEmptyDeck = 0;
	static const int _maxEnergy = 10, _maxHealth = 20;
	TurnData _turnData;

	// Constraints
	Constraints _constraints = Constraints(playerDefaultConstraints);
	Constraints _teamConstraints = Constraints(creatureDefaultConstraints);

	// Card holders
	// The sum of the lengths of these std::vectors/std::stacks is **always** 20
	// because at first, all are empty except the deck which contains... The deck
	// obviously... And then the cards move from one to another but never disappear
	// or are created
	std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
	std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
	std::vector<Creature *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
	std::vector<Card *> _cardGraveyard;  ///< Cards that are discarded (dead creatures, used spells)
	const Card* _lastCasterCard=nullptr; ///<Last card that was used to cast an effect (his or opponent's)

	// Random management
	/// Used for uniformly distributed integer generation
	std::default_random_engine _engine;

	// Effects container
	static std::function<void(Player&, const EffectParamsCollection&)> _effectMethods[P_EFFECTS_COUNT];


	/// User actions
	//TODO: check for each function if Player is the active player, and lock changes to _isActive until end of function
	void useCard(int handIndex); 	///< Use a card
	void attackWithCreature(int boardIndex, int victim);  ///< Attack victim (-1 for opponent) with a card
	void endTurn(); //TODO; define behavior
	/// The game has ended because of some reason (maybe because the user want to quit the game)
	void finishGame(bool hasWon, EndGame::Cause cause);

	/// Effects (private)
	void setConstraint(const EffectParamsCollection& args);
	void pickDeckCards(const EffectParamsCollection& args);
	void loseHandCards(const EffectParamsCollection& args);
	void reviveGraveyardCard(const EffectParamsCollection& args);
	void stealHandCard(const EffectParamsCollection& args);
	void exchgHandCard(const EffectParamsCollection& args);
	void resetEnergy(const EffectParamsCollection& args);
	void changeEnergy(const EffectParamsCollection& args);
	void changeHealth(const EffectParamsCollection& args);

	/// Other private methods
	void exploitCardEffects(Card* usedCard);
	void setTeamConstraint(const Card* usedCard, const EffectParamsCollection& effectArgs);
	void setDeck(const Deck& newDeck);

	void cardDeckToHand(int amount);
	void cardHandToBoard(int handIndex);
	void cardHandToGraveyard(int handIndex);  ///< Move the card at handIndex from the player's hand to the bin
	void cardBoardToGraveyard(int boardIndex);  ///< Move the card at boardIndex from the board to the bin
	void cardGraveyardToHand(int binIndex);
	void cardAddToHand(Card* given);
	Card* cardRemoveFromHand();
	Card* cardExchangeFromHand(Card* given);
	Card* cardExchangeFromHand(Card* given, int handIndex);

	void useCreature(int handIndex, Card *& usedCard);
	void useSpell(int handIndex, Card *& useSpell);

	void logCurrentEnergy();
	void logCurrentHealth();

	void logHandState();
	void logBoardState();
	void logGraveyardState();
	void logCurrentDeck();
	void logOpponentBoardState();
	void logOpponentHealth();
	void logOpponentHandState();

	template <typename CardType>
	void logIdsFromVector(TransferType type, const std::vector<CardType *>& vect);
	void logCardDataFromVector(TransferType type, const std::vector<Card*>& vect);
	void logBoardCreatureDataFromVector(TransferType type, const std::vector<Creature*>& vect);
	void sendValueToClient(TransferType value);
};


#endif  // _PLAYER_HPP

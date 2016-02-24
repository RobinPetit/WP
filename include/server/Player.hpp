#ifndef _PLAYER_HPP
#define _PLAYER_HPP

// std-C++ headers
#include <stack>
#include <random>
#include <array>
#include <functional>
#include <cstddef>
// WizardPoker headers
#include "server/Card.hpp"
#include "server/Spell.hpp"
#include "server/Constraints.hpp"
#include "server/CardData.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class Board;
class Creature;

/// Represents one of the two players for a given game.
class Player
{
public:
	/// Types
	typedef std::size_t ID;

	/// Constructor
	Player(Player::ID id, sf::TcpSocket& socket, sf::TcpSocket& specialSocket);
	void setOpponent(Player* opponent);  // Complementary

	/// Destructor.
	~Player() = default;

	/// Interface for basic gameplay (board)
	void beginGame(bool isActivePlayer);
	void enterTurn(int turn);
	void leaveTurn();
	void useCard(int handIndex); 	///< Use a card
	void attackWithCreature(int boardIndex, int victim);  ///< Attack victim (-1 for opponent) with a card

	/// Interface for applying effects
	//to Player
	void applyEffect(const Card* usedCard, EffectParamsCollection effectArgs);
	//to a Creature
	void applyEffectToCreature(Creature* casterAndSubject, EffectParamsCollection effectArgs); //With ref. to creature
	void applyEffectToCreature(const Card* usedCard, EffectParamsCollection effectArgs, bool randomIndex=false); //With creature index from effectArgs
	//to all Creatures
	void applyEffectToCreatureTeam(const Card* usedCard, EffectParamsCollection effectArgs);

	/// Getters
	int getCreatureConstraint(const Creature& subject, int constraintIDD);
	const Card* getLastCaster();
	Player::ID getID();

private:
	/// Types
	struct TurnData
	{
		int cardsUsed;
		int creaturesPlaced;
		int creatureAttacks;
		int spellCalls;
	};
	constexpr static TurnData _emptyTurnData = {0, 0, 0, 0};

	/// Attributes
	Board* _board;
	Player* _opponent = nullptr;
	Player::ID _id;
	int _energy;
	int _health;

	sf::TcpSocket& _socketToClient;
	sf::TcpSocket& _specialSocketToClient;

	TurnData _turnData;

	// Constraints
	Constraints _constraints = Constraints(P_CONSTRAINT_DEFAULTS, P_CONSTRAINTS_COUNT);
	Constraints _teamConstraints = Constraints(C_CONSTRAINT_DEFAULTS, C_CONSTRAINTS_COUNT);

	// Card holders
	std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
	std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
	std::vector<Creature *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
	std::vector<Card *> _cardBin;  ///< Cards that are discarded (dead creatures, used spells)
	const Card* _lastCasterCard=nullptr; ///<Last card that was used to cast an effect (his or opponent's)

	// Random management
	std::default_random_engine _engine;

	// Effects container
	static std::function<void(Player&, const EffectParamsCollection&)> _effectMethods[P_EFFECTS_COUNT];

	/// Effects (private)
	void setConstraint(const EffectParamsCollection& args);
	void pickDeckCards(const EffectParamsCollection& args);
	void loseHandCards(const EffectParamsCollection& args);
	void reviveBinCard(const EffectParamsCollection& args);
	void stealHandCard(const EffectParamsCollection& args);
	void exchgHandCard(const EffectParamsCollection& args);
	void setEnergy(const EffectParamsCollection& args);
	void changeEnergy(const EffectParamsCollection& args);
	void changeHealth(const EffectParamsCollection& args);

	/// Other private methods
	void exploitCardEffects(Card* usedCard);
	void setTeamConstraint(const Card* usedCard, const EffectParamsCollection& effectArgs);

	void cardDeckToHand(int amount);
	void cardHandToBoard(int handIndex);
	void cardHandToBin(int handIndex);  ///< Move the card at handIndex from the player's hand to the bin
	void cardBoardToBin(int boardIndex);  ///< Move the card at boardIndex from the board to the bin
	void cardBinToHand(int binIndex);
	void cardAddToHand(Card* given);
	Card* cardRemoveFromHand();
	Card* cardExchangeFromHand(Card* given);
	Card* cardExchangeFromHand(Card* given, int handIndex);

	void useCreature(int handIndex, Card *& usedCard);
	void useSpell(int handIndex, Card *& useSpell);

	void sendCurrentEnergy();
	void sendCurrentHealth();
};


#endif  // _PLAYER_HPP

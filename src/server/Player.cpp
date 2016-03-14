// WizardPoker headers
#include "server/Player.hpp"
#include "server/GameThread.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
#include "common/random/RandomInteger.hpp"
// std-C++ headers
#include <iostream>
#include <algorithm>
#include <cassert>
// SFML headers
#include <SFML/Network/Packet.hpp>

/*------------------------------ CONSTRUCTOR AND INIT */
std::function<void(Player&, const EffectParamsCollection&)> Player::_effectMethods[P_EFFECTS_COUNT] =
{
	&Player::setConstraint,
	&Player::pickDeckCards,
	&Player::loseHandCards,
	&Player::reviveGraveyardCard,

	&Player::stealHandCard,
	&Player::exchgHandCard,

	&Player::resetEnergy,
	&Player::changeEnergy,
	&Player::changeHealth,
};

Player::Player(GameThread& gameThread, ServerDatabase& database, userId id):
	_gameThread(gameThread),
	_database(database),
	_id(id),
	_isActive(false)
{
	std::cout << _energy << _health << std::endl;
	// Input sockets are non-blocking, this is easier since the sockets of the
	// two players are separated in the Player instances. So that as soon as a
	// packet is received, the packet is handled, rather than waiting that the
	// other Player instance receives a packet from the other client.
	_socketToClient.setBlocking(false);
}

void Player::setOpponent(Player* opponent)
{
	_opponent = opponent;
}

int Player::getHealth() const
{
	return _health;
}

std::vector<std::unique_ptr<Card>>::size_type Player::getHandSize() const
{
	return _cardHand.size();
}

userId Player::getId() const
{
	return _id;
}

const std::vector<std::unique_ptr<Creature>>& Player::getBoard() const
{
	return _cardBoard;
}

bool Player::thereAreBoardChanges()
{
	return _pendingBoardChanges.getDataSize() > 0;
}

sf::Packet Player::getBoardChanges()
{
	sf::Packet res{_pendingBoardChanges};
	_pendingBoardChanges.clear();
	return res;
}

void Player::setDeck(const Deck& newDeck)
{
	std::vector<std::unique_ptr<Card>> loadedCards{Deck::size};
	for(std::size_t i{0}; i < Deck::size; ++i)
	{
		const cardId card{newDeck.getCard(i)};
		// FIXME For now, we consider that cardId <= 10 are creatures,
		// and higher cardId are spells. THIS SHOULD BE FIXED.
		if(card <= 10)
		{
			const CreatureData& creat = ALL_CREATURES[card-1];
			loadedCards[i].reset(new Creature(card, *this, creat.cost, creat.attack, creat.health, creat.shield, creat.shieldType, creat.effects));
		}
		else
		{
			const SpellData& spell = ALL_SPELLS[card - 11];
			loadedCards[i].reset(new Spell(card, spell.cost, spell.effects));
		}
	}
	//Deterministic behavior is best while testing
	//TODO: re-enable shuffling when required
	std::shuffle(loadedCards.begin(), loadedCards.end(), std::mt19937(std::random_device()()));
	for(auto& cardInShuffledDeck: loadedCards)
		_cardDeck.push(std::move(cardInShuffledDeck));
}

void Player::receiveDeck()
{
	sf::Packet deckPacket;
	TransferType type;
	std::string deckName;

	// Have the socket blocking because the data is needed to go further
	_socketToClient.setBlocking(true);
	_socketToClient.receive(deckPacket);
	_socketToClient.setBlocking(false);
	deckPacket >> type;
	if(type != TransferType::GAME_PLAYER_GIVE_DECK_NAMES)
		throw std::runtime_error("Unable to get player " + std::to_string(getId()) + " deck");
	deckPacket >> deckName;

	setDeck(_database.getDeckByName(getId(), deckName));
}

void Player::beginGame(bool isActivePlayer)
{
	// init Player's data
	cardDeckToHand(_initialAmountOfCards);  // calls logHandState
	// log & send
	logCurrentEnergy();
	logCurrentHealth();
	logOpponentHealth();
	logCurrentDeck();
	// Do not call `logOpponentHandState()` because players are initialized one after another.
	// So when (first player).beginGame is called, (second player).hand is still empty.
	// Thus to ensure the right value is sent, it is sent directly
	_pendingBoardChanges << TransferType::GAME_OPPONENT_HAND_UPDATED << static_cast<sf::Uint32>(_initialAmountOfCards);
	_socketToClient.send(_pendingBoardChanges);
	_pendingBoardChanges.clear();

	sf::Packet packet;
	packet << TransferType::GAME_STARTING << (isActivePlayer ? TransferType::GAME_PLAYER_ENTER_TURN : TransferType::GAME_PLAYER_LEAVE_TURN);
	_socketToClient.send(packet);
	_isActive.store(isActivePlayer);
}

void Player::enterTurn(int turn)
{
	_isActive.store(true); //Player has become active

	_turnData = _emptyTurnData;  // Reset the turn data
	_turnData.turnCount = turn;  // Store turn number

	if(_cardDeck.empty())
	{
		_turnsSinceEmptyDeck++;
		if (_turnsSinceEmptyDeck == 10)
			finishGame(false, EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK);
	}

	//Player's turn-based constraints
	// do not pick a card at first turn
	if(turn != 1)
		cardDeckToHand(_constraints.getConstraint(PC_TURN_CARDS_PICKED));
	resetEnergy({_constraints.getConstraint(PC_TURN_ENERGY_INIT_CHANGE)});
	changeEnergy({_constraints.getConstraint(PC_TURN_ENERGY_CHANGE)});
	changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE)});
	if(_cardDeck.empty())
		changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE_DECK_EMPTY)});

	//Will call creature's turn-based constraints
	for(unsigned i = 0; i < _cardBoard.size(); i++)
		_cardBoard.at(i)->enterTurn();
}

void Player::leaveTurn()
{
	_isActive.store(false); //Player is no longer active

	//Time out player constraints
	_constraints.timeOutConstraints();
	_teamConstraints.timeOutConstraints();

	//Time out player's creature's constraints
	for(unsigned i = 0; i < _cardBoard.size(); i++)
		_cardBoard.at(i)->leaveTurn();
}

void Player::finishGame(bool hasWon, EndGame::Cause cause)
{
	_gameThread.endGame(hasWon ? getId() : _opponent->getId(), cause);
}


/*------------------------------ CLIENT INTERFACE */
sf::Socket::Status Player::tryReceiveClientInput()
{
	sf::Packet playerActionPacket;
	const sf::Socket::Status status{_socketToClient.receive(playerActionPacket)};
	// If no data was received
	if(status != sf::Socket::Done)
		return status;

	TransferType type;
	playerActionPacket >> type;

	if(type == TransferType::GAME_QUIT_GAME)
		finishGame(false, EndGame::Cause::QUITTED);

	else
	{
		// These methods are not allowed for passive player
		if (_isActive.load() == false)
		{
			std::cout << "Passive player tried to cheat, perma ban ?\n";
			return status;
		}

		switch(type)
		{
			case TransferType::GAME_PLAYER_LEAVE_TURN:
				endTurn();
				break;

			case TransferType::GAME_USE_CARD:
			{
				sf::Int32 cardIndex;
				playerActionPacket >> cardIndex;
				useCard(static_cast<int>(cardIndex));
				break;
			}

			case TransferType::GAME_ATTACK_WITH_CREATURE:
			{
				sf::Int32 attackerIndex, victimIndex;
				playerActionPacket >> attackerIndex >> victimIndex;
				attackWithCreature(static_cast<int>(attackerIndex), static_cast<int>(victimIndex));
				break;
			}

			default:
				std::cerr << "Player::tryReceiveClientInput error: wrong packet header ("
				          << static_cast<sf::Uint32>(type) << "), expected in-game action header.\n";
				break;
		}
	}
	return status;
}

/// \network sends to client one of the following:
///	 + ACKNOWLEDGE if the card was successfully used
///	 + GAME_CARD_LIMIT_TURN_REACHED if the user cannot play cards for this turn
///	 + GAME_NOT_ENOUGH_ENERGY if the user has not enough energy to play this card
///	 + FAILURE  if the specialized type of the card (spell/creature) cannot be played anymore for this turn
void Player::useCard(int handIndex)
{
	Card* usedCard;
	try //check the input
	{
		 usedCard = _cardHand.at(handIndex).get();
	 }
	catch(std::out_of_range&)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}
	// check if player has enough energy to use the card
	if(usedCard->getEnergyCost() > _energy)
	{
		sendValueToClient(TransferType::GAME_NOT_ENOUGH_ENERGY);
		return;
	}

	// check if player is allowed to use more cards
	if(_constraints.getConstraint(PC_TEMP_CARD_USE_LIMIT) == _turnData.cardsUsed)
	{
		sendValueToClient(TransferType::GAME_CARD_LIMIT_TURN_REACHED);
		return;
	}

	_lastCasterCard = usedCard;
	_opponent->_lastCasterCard = usedCard;

	(this->*(usedCard->isCreature() ? &Player::useCreature : &Player::useSpell))(handIndex, usedCard);
	logHandState();
	_opponent->logOpponentHandState();
	logCurrentEnergy();
}

////////////////////// specialized card cases

void Player::useCreature(int handIndex, Card* usedCard)
{
	// check if player is allowed to place a creature
	if (_constraints.getConstraint(PC_TEMP_CREATURE_PLACING_LIMIT) == _turnData.creaturesPlaced)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}

	_turnData.cardsUsed++;
	_turnData.creaturesPlaced++;
	_energy -= usedCard->getEnergyCost();

	cardHandToBoard(handIndex);
	exploitCardEffects(usedCard);
	sendValueToClient(TransferType::ACKNOWLEDGE);
	logBoardState();
	_opponent->logOpponentBoardState();
}

void Player::useSpell(int handIndex, Card* usedCard)
{
	// check if player is allowed to call a spell
	if (_constraints.getConstraint(PC_TEMP_SPELL_CALL_LIMIT) == _turnData.spellCalls)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}

	_turnData.cardsUsed++;
	_turnData.spellCalls++;
	_energy -= usedCard->getEnergyCost();

	cardHandToGraveyard(handIndex);
	exploitCardEffects(usedCard);
	sendValueToClient(TransferType::ACKNOWLEDGE);
	logHandState();
}

void Player::attackWithCreature(int attackerIndex, int victimIndex)
{
	Creature* attacker;
	Creature* victim;
	try //check the input
	{
		attacker = _cardBoard.at(attackerIndex).get();
		if(victimIndex >= 0)
			victim = _opponent->_cardBoard.at(victimIndex).get();
	}
	catch (std::out_of_range&)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}

	// check if we have enough energy to use this card
	if(attacker->getEnergyCost() > _energy)
	{
		sendValueToClient(TransferType::GAME_NOT_ENOUGH_ENERGY);
		return;
	}

	// check if player is allowed to attack
	if(_constraints.getConstraint(PC_TEMP_CREATURE_ATTACK_LIMIT) == _turnData.creatureAttacks)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}

	_turnData.creatureAttacks++;
	_energy -= attacker->getEnergyCost();

	if(victimIndex < 0)
	{
		std::vector<int> params{{PE_CHANGE_HEALTH, -(attacker->getAttack())}}; // \TODO: I don't think we should use effects this way
		_opponent->applyEffectToSelf(params);  //no forced attacks on opponent
		logOpponentHealth();
	}
	else
	{
		attacker->makeAttack(*victim);
		logOpponentBoardState();
		logBoardState();  // If an attack is returned to the attacker, the board changes
	}
	sendValueToClient(TransferType::ACKNOWLEDGE);
}

void Player::endTurn()
{
	_gameThread.swapTurns();
}

/*------------------------------ EFFECTS INTERFACE */
void Player::applyEffect(Card* usedCard, EffectParamsCollection effectArgs)
{
	int subject;  // who the effect applies to
	try  // check the input
	{
		subject = effectArgs.at(0);
		effectArgs.erase(effectArgs.begin());
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	_lastCasterCard = usedCard;  // remember last used card
	_opponent->_lastCasterCard = usedCard; // same for opponent

	switch(subject)
	{
		case PLAYER_SELF:  //passive player
			askUserToSelectCards({});
			applyEffectToSelf(effectArgs);
			break;

		case PLAYER_OPPO:  //active player
			askUserToSelectCards({});
			_opponent->applyEffectToSelf(effectArgs);
			break;

		case CREATURE_SELF_THIS:  //active player's creature that was used
		{
			askUserToSelectCards({});
			Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
			applyEffectToCreature(usedCreature, effectArgs);
		}
			break;

		case CREATURE_SELF_INDX:  //active player's creature at given index
			if(not _cardBoard.empty())
				applyEffectToCreature(effectArgs, askUserToSelectCards({CardToSelect::SELF_BOARD}));
			break;

		case CREATURE_SELF_RAND:  //active player's creature at random index
			askUserToSelectCards({});
			if(not _cardBoard.empty())
				applyEffectToCreature(effectArgs, getRandomBoardIndexes({CardToSelect::SELF_BOARD}));
			break;

		case CREATURE_SELF_TEAM:  //active player's team of creatures
			askUserToSelectCards({});
			applyEffectToCreatureTeam(effectArgs);
			break;

		case CREATURE_OPPO_INDX:	//passive player's creature at given index
			if(not _opponent->_cardBoard.empty())
				_opponent->applyEffectToCreature(effectArgs, askUserToSelectCards({CardToSelect::OPPO_BOARD}));
			break;

		case CREATURE_OPPO_RAND:	//passive player's creature at random index
			askUserToSelectCards({});
			if(not _opponent->_cardBoard.empty())
				_opponent->applyEffectToCreature(effectArgs, getRandomBoardIndexes({CardToSelect::OPPO_BOARD}));
			break;

		case CREATURE_OPPO_TEAM:	//passive player's team of creatures
			askUserToSelectCards({});
			_opponent->applyEffectToCreatureTeam(effectArgs);
			break;

		default:
			throw std::runtime_error("Effect subject not valid");
	}
}

void Player::applyEffectToSelf(EffectParamsCollection& effectArgs)
{
	int method = effectArgs.front();  //what method is used
	effectArgs.erase(effectArgs.begin());

	_effectMethods[method](*this, effectArgs);  //call method on self
}

void Player::applyEffectToCreature(Creature* casterAndSubject, EffectParamsCollection& effectArgs)
{
	casterAndSubject->applyEffectToSelf(effectArgs); //call method on effect subject (same as caster)
}

void Player::applyEffectToCreature(EffectParamsCollection& effectArgs, const std::vector<int>& boardIndexes)
{
	_cardBoard.at(boardIndexes.at(0))->applyEffectToSelf(effectArgs);
}

void Player::applyEffectToCreatureTeam(EffectParamsCollection& effectArgs)
{
	//If the effect consists in setting a constraint
	if(effectArgs.at(0) == CE_SET_CONSTRAINT)
	{
		effectArgs.erase(effectArgs.begin()); //remove the method value
		setTeamConstraint(effectArgs); //set a team constraint instead of individual ones
	}
	else //other effects just get applied to each creature individually
		for(auto& card : _cardBoard)
			card->applyEffectToSelf(effectArgs);
}

/*------------------------------ GETTERS */
int Player::getCreatureConstraint(const Creature& subject, int constraintId) const
{
	//returns the value respecting both the creature and the whole team's constraints
	int creatureValue = subject.getPersonalConstraint(constraintId);
	return _teamConstraints.getOverallConstraint(constraintId, creatureValue);
}

const Card* Player::getLastCaster() const
{
	return _lastCasterCard;
}

sf::TcpSocket& Player::getSocket()
{
    return _socketToClient;
}

/*------------------------------ EFFECTS (PRIVATE) */
void Player::setConstraint(const EffectParamsCollection& args)
{
	int constraintId; //constraint to set
	int value; //value to give to it
	int turns; //for how many turns
	int casterOptions; //whether the constraint depends on its caster being alive
	try //check the input
	{
		constraintId=args.at(0);
		value=args.at(1);
		turns=args.at(2);
		casterOptions=args.at(3);
		if (constraintId<0 or constraintId>=P_CONSTRAINTS_COUNT or turns<0)
			throw std::out_of_range("");
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	switch(casterOptions)
	{
		case IF_CASTER_ALIVE:
			_constraints.setConstraint(constraintId, value, turns, dynamic_cast<const Creature*>(getLastCaster()));
			break;

		default:
			_constraints.setConstraint(constraintId, value, turns);
			break;
	}
}

void Player::pickDeckCards(const EffectParamsCollection& args)
{
	int amount;//amount of cards
	try //check the input
	{
		amount=args.at(0);
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
	cardDeckToHand(amount);
}

void Player::loseHandCards(const EffectParamsCollection& args)
{
	try  //check the input
	{
		for(int amount{args.at(0)}; not _cardHand.empty() and amount > 0; amount--)
			cardHandToGraveyard(getRandomIndex(_cardHand));
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
}

void Player::reviveGraveyardCard(const EffectParamsCollection& args)
{
	int binIndex;  //what card to revive
	try  //check the input
	{
		binIndex = args.at(0);
		_cardGraveyard.at(binIndex);
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
	cardGraveyardToHand(binIndex);
}

void Player::stealHandCard(const EffectParamsCollection& /* args */)
{
	//no arguments
	cardAddToHand(_opponent->cardRemoveFromHand());
}

/// \network sends to user one of the following:
///	 + FAILURE if opponent has no card in his hand
///	 + ACKNOWLEDGE if card has been swapped
void Player::exchgHandCard(const EffectParamsCollection& args)
{
	int myCardIndex; //card to exchange
	sf::Packet packet;
	std::unique_ptr<Card> myCard;

	try //check the input
	{
		myCardIndex = args.at(0);
		myCard = std::move(_cardHand.at(myCardIndex));
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	std::unique_ptr<Card> hisCard(_opponent->cardExchangeFromHand(std::move(myCard)));

	if(hisCard == nullptr)
	{
		packet << TransferType::FAILURE;
		// Remove the card that was given to the opponent from the hand vector
		_cardHand.erase(_cardHand.begin() + myCardIndex);
	}
	else
	{
		cardExchangeFromHand(std::move(hisCard), myCardIndex);
		packet << TransferType::ACKNOWLEDGE;
	}
	_socketToClient.send(packet); //Shouldn't this be called before cardExchangeFromHand ?
}

void Player::resetEnergy(const EffectParamsCollection& args)
{
	try //check the input
	{
		_energyInit += args.at(0);
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
	if(_energyInit < 0)
		_energyInit = 0;
	else if(_energyInit > _maxEnergy)
		_energyInit = _maxEnergy;
	_energy = _energyInit;
	logCurrentEnergy();
}

void Player::changeEnergy(const EffectParamsCollection& args)
{
	try //check the input
	{
		_energy += args.at(0);
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
	if(_energy < 0)
		_energy = 0;
	else if(_energy > _maxEnergy)
		_energy = _maxEnergy;
	logCurrentEnergy();
}

void Player::changeHealth(const EffectParamsCollection& args)
{
	try //check the input
	{
		_health += args.at(0);
		if(_health < 0)
		{
			_health = 0;
			finishGame(false, EndGame::Cause::OUT_OF_HEALTH);
		}
		else if(_health > _maxHealth)
		{
			_health = _maxHealth;
			logCurrentHealth();
			_opponent->logOpponentHealth();
		}
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

}


///////// loggers

void Player::logCurrentEnergy()
{
	// cast to be sure that the right amount of bits is sent and received
	_pendingBoardChanges << TransferType::GAME_PLAYER_ENERGY_UPDATED << static_cast<sf::Uint32>(_energy);
}

void Player::logCurrentHealth()
{
	// cast to be sure that the right amount of bits is sent and received
	_pendingBoardChanges << TransferType::GAME_PLAYER_HEALTH_UPDATED << static_cast<sf::Uint32>(_health);
}

void Player::logOpponentHealth()
{
	_pendingBoardChanges << TransferType::GAME_OPPONENT_HEALTH_UPDATED << static_cast<sf::Uint32>(_opponent->getHealth());
}

void Player::logCurrentDeck()
{
	_pendingBoardChanges << TransferType::GAME_DECK_UPDATED << static_cast<sf::Uint32>(_cardDeck.size());
}

void Player::logOpponentHandState()
{
	_pendingBoardChanges << TransferType::GAME_OPPONENT_HAND_UPDATED << static_cast<sf::Uint32>(_opponent->getHandSize());
}

/*--------------------------- PRIVATE */

void Player::exploitCardEffects(Card* usedCard)
{
	const std::vector<EffectParamsCollection>& effects(usedCard->getEffects());
	for(const auto& effectArgs: effects) //for each effect of the card
		applyEffect(usedCard, effectArgs); //apply it
}

void Player::setTeamConstraint(const EffectParamsCollection& args)
{
	int constraintId;  // constraint to set
	int value;  // value to give to it
	int turns;  // for how many turns
	int casterOptions;  // whether the constraint depends on its caster being alive
	try  // check the input
	{
		constraintId = args.at(0);
		value = args.at(1);
		turns = args.at(2);
		casterOptions = args.at(3);
		if(constraintId < 0 or constraintId >= C_CONSTRAINTS_COUNT or turns < 0)
			throw std::out_of_range("");
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	switch(casterOptions)
	{
		case IF_CASTER_ALIVE:
			_constraints.setConstraint(constraintId, value, turns, dynamic_cast<const Creature*>(getLastCaster()));
			break;

		default:
			_constraints.setConstraint(constraintId, value, turns);
			break;
	}
}

void Player::cardDeckToHand(int amount)
{
	while(not _cardDeck.empty() and amount > 0)
	{
		amount--;
		_cardHand.push_back(std::move(_cardDeck.top()));
		_cardDeck.pop();
	}
	logHandState();
	_opponent->logOpponentHandState();
	logCurrentDeck();
}

void Player::cardHandToBoard(int handIndex)
{
	// Release the ownership of the hand, cast to a Creature pointer and give it to the board
	_cardBoard.push_back(std::unique_ptr<Creature>(dynamic_cast<Creature*>(_cardHand.at(handIndex).release())));
	_cardBoard.back()->moveToBoard();
	_cardHand.erase(_cardHand.begin() + handIndex);
	logHandState();
	_opponent->logOpponentHandState();
	logBoardState();
	_opponent->logOpponentBoardState();
}

void Player::cardHandToGraveyard(int handIndex)
{
	_cardGraveyard.push_back(std::move(_cardHand.at(handIndex)));
	_cardHand.erase(_cardHand.begin() + handIndex);
	logHandState();
	_opponent->logOpponentHandState();
	logGraveyardState();
}

void Player::cardBoardToGraveyard(int boardIndex)
{
	_cardBoard.at(boardIndex)->removeFromBoard();
	// Release the ownership of the board, cast to a Card pointer and give it to the graveyard
	_cardGraveyard.push_back(std::unique_ptr<Card>(static_cast<Card*>(_cardBoard.at(boardIndex).release())));
	_cardBoard.erase(_cardBoard.begin() + boardIndex);
	logBoardState();
	_opponent->logOpponentBoardState();
	logGraveyardState();
}

void Player::cardGraveyardToHand(int binIndex)
{
	_cardHand.push_back(std::move(_cardGraveyard.at(binIndex)));
	_cardGraveyard.erase(_cardGraveyard.begin() + binIndex);
	logGraveyardState();
	logHandState();
	_opponent->logOpponentHandState();
}

void Player::cardAddToHand(std::unique_ptr<Card> givenCard)
{
	if(givenCard != nullptr)
	{
		_cardHand.push_back(std::move(givenCard));
		logHandState();
		_opponent->logOpponentHandState();
	}
}

std::unique_ptr<Card> Player::cardRemoveFromHand()
{
	if(_cardHand.empty())
		return nullptr;
	int handIndex = getRandomIndex(_cardHand);
	std::unique_ptr<Card> stolenCard(std::move(_cardHand[handIndex]));
	_cardHand.erase(_cardHand.begin() + handIndex);
	logHandState();
	_opponent->logOpponentHandState();
	return std::move(stolenCard);
}

std::unique_ptr<Card> Player::cardExchangeFromHand(std::unique_ptr<Card> givenCard)
{
	int handIndex = getRandomIndex(_cardHand);
	return std::move(cardExchangeFromHand(std::move(givenCard), handIndex));
}

std::unique_ptr<Card> Player::cardExchangeFromHand(std::unique_ptr<Card> givenCard, int handIndex)
{
	if(_cardHand.empty())
		return nullptr;
	std::swap(givenCard, _cardHand[handIndex]);
	logHandState();
	_opponent->logOpponentHandState();
	return std::move(givenCard);
}

void Player::logHandState()
{
	logCardDataFromVector(TransferType::GAME_HAND_UPDATED, _cardHand);
}

void Player::logBoardState()
{
	logBoardCreatureDataFromVector(TransferType::GAME_BOARD_UPDATED, _cardBoard);
}

void Player::logOpponentBoardState()
{
	logBoardCreatureDataFromVector(TransferType::GAME_OPPONENT_BOARD_UPDATED, _opponent->getBoard());
}

void Player::logGraveyardState()
{
	logCardDataFromVector(TransferType::GAME_GRAVEYARD_UPDATED, _cardGraveyard);
}

// use a template to handle both Card and Creature pointers
template <typename CardType>
void Player::logIdsFromVector(TransferType type, const std::vector<std::unique_ptr<CardType>>& vect)
{
	std::vector<sf::Uint32> cardIds(vect.size());
	for(std::size_t i{0}; i < vect.size(); ++i)
		cardIds[i] = vect[i]->getId();

	_pendingBoardChanges << type << cardIds;
}

void Player::logCardDataFromVector(TransferType type, const std::vector<std::unique_ptr<Card>>& vect)
{
	std::vector<CardData> cards;
	for(std::size_t i = 0U; i < vect.size(); ++i)
	{
		CardData data;
		data.id = vect.at(i)->getId();
		cards.push_back(data);
	}
	_pendingBoardChanges << type << cards;
}

void Player::logBoardCreatureDataFromVector(TransferType type, const std::vector<std::unique_ptr<Creature>>& vect)
{
	std::vector<BoardCreatureData> boardCreatures;
	for (std::size_t i = 0U; i<vect.size(); i++)
	{
		BoardCreatureData data;
		Creature& creat = *vect.at(i);
		data.id = creat.getId();
		data.attack = creat.getAttack();
		data.health = creat.getHealth();
		data.shield = creat.getShield();
		int shieldType = creat.getShieldType();
		switch(shieldType)
		{
			case SHIELD_NONE:
				data.shieldType = "none";
				break;

			case SHIELD_BLUE:
				data.shieldType = "blue";
				break;

			case SHIELD_ORANGE:
				data.shieldType = "orange";
				break;

			case SHIELD_LEGENDARY:
				data.shieldType = "legendary";
				break;
		}
		boardCreatures.push_back(data);
	}
	// std::vector transmission in packet is defined in common/sockets/PacketOverload.hpp
	_pendingBoardChanges << type << boardCreatures;
}

//////////

// TODO: handle the case where the user doesn't give and his turn finishes
std::vector<int> Player::askUserToSelectCards(const std::vector<CardToSelect>& selection)
{
	sf::Packet packet;
	packet << selection;
	_socketToClient.send(packet);
	std::vector<sf::Uint32> indices(selection.size());
	// have the socket blocking because the answer is expected at this very moment
	_socketToClient.setBlocking(true);
	_socketToClient.receive(packet);
	_socketToClient.setBlocking(false);
	packet >> indices;
	assert(indices.size() == selection.size());
	// convert the sf::Uint32 received on the network by implementation-defined integers
	std::vector<int> ret(selection.size());
	for(std::size_t i{0U}; i < indices.size(); ++i)
		ret[i] = static_cast<int>(indices[i]);
	return ret;
}

template <typename T>
inline int Player::getRandomIndex(const std::vector<T>& vector)
{
	if(vector.empty())
		throw std::out_of_range("Cannot generate a random index for an empty vector.");
	return _gameThread.getGenerator().next(static_cast<int>(vector.size()));
}

std::vector<int> Player::getRandomBoardIndexes(const std::vector<CardToSelect>& selection)
{
	std::vector<int> indices(selection.size());
	for(std::size_t i{0}; i < selection.size(); i++)
	{
		switch(selection.at(i))
		{
			case CardToSelect::SELF_BOARD:
				indices[i] = getRandomIndex(_cardBoard);
				break;

			case CardToSelect::SELF_HAND:
				indices[i] = getRandomIndex(_cardHand);
				break;

			case CardToSelect::OPPO_BOARD:
				indices[i] = getRandomIndex(_opponent->_cardBoard);
				break;

			case CardToSelect::OPPO_HAND:
				indices[i] = getRandomIndex(_opponent->_cardHand);
				break;
		}
	}
	return indices;
}

void Player::sendValueToClient(TransferType transferType)
{
	sf::Packet packet;
	packet << transferType;
	_socketToClient.send(packet);
}

// WizardPoker headers
#include "server/Player.hpp"
#include "server/GameThread.hpp"
#include "server/Creature.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
// std-C++ headers
#include <iostream>
#include <algorithm>
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

std::vector<Card *>::size_type Player::getHandSize() const
{
	return _cardHand.size();
}

userId Player::getID() const
{
	return _id;
}

const std::vector<Creature *>& Player::getBoard() const
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
	std::vector<Card *> loadedCards{Deck::size};
	for(std::size_t i{0}; i < Deck::size; ++i)
	{
		const cardId card{newDeck.getCard(i)};
		// FIXME For now, we consider that cardId <= 10 are creatures,
		// and higher cardId are spells. THIS SHOULD BE FIXED.
		if(card <= 10)
		{
			CreatureData creat = ALL_CREATURES[card-1];
			loadedCards[i] = new Creature(card, creat.cost, creat.attack, creat.health, creat.shield, creat.shieldType, creat.effects);
		}
		else
		{
			SpellData spell = ALL_SPELLS[card - 11];
			loadedCards[i] = new Spell(card, spell.cost, spell.effects);
		}
	}
	//Deterministic behavior is best while testing
	//TODO: re-enable shuffling when required
	std::shuffle(loadedCards.begin(), loadedCards.end(), std::mt19937(std::random_device()()));
	for(const auto& cardInShuffledDeck: loadedCards)
		_cardDeck.push(cardInShuffledDeck);
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
		throw std::runtime_error("Unable to get player " + std::to_string(getID()) + " deck");
	deckPacket >> deckName;

	setDeck(_database.getDeckByName(getID(), deckName));
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

	if (_cardDeck.empty())
	{
		_turnsSinceEmptyDeck++;
		if (_turnsSinceEmptyDeck==10)
			finishGame(false, EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK);
	}

	//Player's turn-based constraints
	cardDeckToHand(_constraints.getConstraint(PC_TURN_CARDS_PICKED));
	resetEnergy({_constraints.getConstraint(PC_TURN_ENERGY_INIT_CHANGE)});
	changeEnergy({_constraints.getConstraint(PC_TURN_ENERGY_CHANGE)});
	changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE)});
	if (_cardDeck.empty())
		changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE_DECK_EMPTY)});

	//Will call creature's turn-based constraints
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->enterTurn();
}

void Player::leaveTurn()
{
	_isActive.store(false); //Player is no longer active

	//Time out player constraints
	_constraints.timeOutConstraints();
	_teamConstraints.timeOutConstraints();

	//Time out player's creature's constraints
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->leaveTurn();
}

void Player::finishGame(bool hasWon, EndGame::Cause cause)
{
	_gameThread.endGame(hasWon ? getID() : _opponent->getID(), cause);
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
	// Be sure this is the active player that sent input
	else if(_isActive.load())
	{
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
				std::cout << "Player::tryReceiveClientInput error: wrong packet header, "
				             "expected in-game action header.\n";
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
	try //check the input
	{
		_cardHand.at(handIndex);
	}
	catch (std::out_of_range&)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}
	Card* usedCard = _cardHand.at(handIndex);
	//Check if we have enough energy to use this card
	if (usedCard->getEnergyCost() > _energy)
	{
		sendValueToClient(TransferType::GAME_NOT_ENOUGH_ENERGY);
		return;
	}

	//Check if we have the right to put more cards (constraint)
	if (_constraints.getConstraint(PC_TEMP_CARD_USE_LIMIT) == _turnData.cardsUsed)
	{
		sendValueToClient(TransferType::GAME_CARD_LIMIT_TURN_REACHED);
		return;
	}

	_energy -= usedCard->getEnergyCost();
	(*this.*(usedCard->isCreature() ? &Player::useCreature : &Player::useSpell))(handIndex, usedCard);
	logHandState();
	_opponent->logOpponentHandState();
}

////////////////////// specialized card cases

void Player::useCreature(int handIndex, Card *& usedCard)
{
	if (_constraints.getConstraint(PC_TEMP_CREATURE_PLACING_LIMIT) == _turnData.creaturesPlaced)
		sendValueToClient(TransferType::FAILURE);
	else
	{
		_turnData.cardsUsed++;
		_turnData.creaturesPlaced++;
		cardHandToBoard(handIndex);
		exploitCardEffects(usedCard);
		sendValueToClient(TransferType::ACKNOWLEDGE);
	}
	logBoardState();
	_opponent->logOpponentBoardState();
}

void Player::useSpell(int handIndex, Card *& usedCard)
{
	if (_constraints.getConstraint(PC_TEMP_SPELL_CALL_LIMIT) == _turnData.spellCalls)
		sendValueToClient(TransferType::FAILURE);
	else
	{
		_turnData.cardsUsed++;
		_turnData.spellCalls++;
		cardHandToGraveyard(handIndex);
		exploitCardEffects(usedCard);
		sendValueToClient(TransferType::ACKNOWLEDGE);
	}
	logGraveyardState();
}

void Player::attackWithCreature(int attackerIndex, int victimIndex)
{
	try //check the input
	{
		_cardHand.at(attackerIndex);
		_opponent->_cardHand.at(victimIndex);
	}
	catch (std::out_of_range&)
	{
		sendValueToClient(TransferType::FAILURE);
		return;
	}

	//Check if we have enough energy to use this card
	Creature* attacker = _cardBoard.at(attackerIndex);
	if (attacker->getEnergyCost() > _energy)
	{
		sendValueToClient(TransferType::GAME_NOT_ENOUGH_ENERGY);
		return;
	}
	_energy -= attacker->getEnergyCost();

	if (_constraints.getConstraint(PC_TEMP_CREATURE_ATTACK_LIMIT) == _turnData.creatureAttacks)
		sendValueToClient(TransferType::FAILURE);
	else
	{
		if (victimIndex<0)
			_opponent->applyEffectToSelf(attacker, {PE_CHANGE_HEALTH, -attacker->getAttack()}); //no forced attacks on opponent
		else
			attacker->makeAttack(*_opponent->_cardBoard.at(victimIndex));
		sendValueToClient(TransferType::ACKNOWLEDGE);
	}
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
		subject=effectArgs.at(0);
		effectArgs.erase(effectArgs.begin());
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	switch (subject)
	{
		case PLAYER_SELF:	//passive player
			askUserToSelectCards({});
			applyEffectToSelf(usedCard, effectArgs);
			break;

		case PLAYER_OPPO:	//active player
			askUserToSelectCards({});
			_opponent->applyEffectToSelf(usedCard, effectArgs);
			break;
		case CREATURE_SELF_THIS:	//active player's creature that was used
		{
			askUserToSelectCards({});
			Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
			applyEffectToCreature(usedCreature, effectArgs);
		}
			break;
		case CREATURE_SELF_INDX:	//active player's creature at given index
			applyEffectToCreature(usedCard, effectArgs, askUserToSelectCards({CardToSelect::SELF_BOARD}));
			break;

		case CREATURE_SELF_RAND:	//active player's creature at random index
			askUserToSelectCards({});
			applyEffectToCreature(usedCard, effectArgs, getRandomBoardIndexes({CardToSelect::SELF_BOARD}));
			break;

		case CREATURE_SELF_TEAM:	//active player's team of creatures
			askUserToSelectCards({});
			applyEffectToCreatureTeam(usedCard, effectArgs);
			break;

		case CREATURE_OPPO_INDX:	//passive player's creature at given index
			_opponent->applyEffectToCreature(usedCard, effectArgs, askUserToSelectCards({CardToSelect::OPPO_BOARD}));
			break;

		case CREATURE_OPPO_RAND:	//passive player's creature at random index
			askUserToSelectCards({});
			_opponent->applyEffectToCreature(usedCard, effectArgs, getRandomBoardIndexes({CardToSelect::OPPO_BOARD}));
			break;

		case CREATURE_OPPO_TEAM:	//passive player's team of creatures
			askUserToSelectCards({});
			_opponent->applyEffectToCreatureTeam(usedCard, effectArgs);
			break;
		default:
			throw std::runtime_error("Effect subject not valid");
	}
}

void Player::applyEffectToSelf(const Card* usedCard, EffectParamsCollection effectArgs)
{
	_lastCasterCard = usedCard; //remember last used card

	int method = effectArgs.front(); //what method is used
	effectArgs.erase(effectArgs.begin());

	_effectMethods[method](*this, effectArgs); //call method on self
}

void Player::applyEffectToCreature(Creature* casterAndSubject, EffectParamsCollection effectArgs)
{
	_lastCasterCard = casterAndSubject; //remember last used card
	casterAndSubject->applyEffectToSelf(effectArgs); //call method on effect subject (same as caster)
}

void Player::applyEffectToCreature(const Card* usedCard, EffectParamsCollection effectArgs, std::vector<int> boardIndexes)
{
	_lastCasterCard = usedCard; //remember last used card
	_cardBoard.at(boardIndexes.at(0))->applyEffectToSelf(effectArgs);
}

void Player::applyEffectToCreatureTeam(const Card* usedCard, EffectParamsCollection effectArgs)
{
	_lastCasterCard = usedCard;

	//If the effect consists in setting a constraint
	if (effectArgs.at(0) == CE_SET_CONSTRAINT)
	{
		effectArgs.erase(effectArgs.begin()); //remove the method value
		setTeamConstraint(usedCard, effectArgs); //set a team constraint instead of individual ones
	}
	else //other effects just get applied to each creature individually
		for (unsigned i=0; i<_cardBoard.size(); i++)
			_cardBoard.at(i)->applyEffectToSelf(effectArgs);
}

/*------------------------------ GETTERS */
int Player::getCreatureConstraint(const Creature& subject, int constraintID) const
{
	//returns the value respecting both the creature and the whole team's constraints
	int creatureValue = subject.getPersonalConstraint(constraintID);
	return _teamConstraints.getOverallConstraint(constraintID, creatureValue);
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
	int constraintID; //constraint to set
	int value; //value to give to it
	int turns; //for how many turns
	int casterOptions; //whether the constraint depends on its caster being alive
	try //check the input
	{
		constraintID=args.at(0);
		value=args.at(1);
		turns=args.at(2);
		casterOptions=args.at(3);
		if (constraintID<0 or constraintID>=P_CONSTRAINTS_COUNT or turns<0)
			throw std::out_of_range("");
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	switch (casterOptions)
	{
		case IF_CASTER_ALIVE:
			_constraints.setConstraint(constraintID, value, turns, dynamic_cast<const Creature*>(getLastCaster()));
			break;
		default:
			_constraints.setConstraint(constraintID, value, turns);
	}
}

void Player::pickDeckCards(const EffectParamsCollection& args)
{
	int amount;//amount of cards
	try //check the input
	{
		amount=args.at(0);
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
	cardDeckToHand(amount);
}

void Player::loseHandCards(const EffectParamsCollection& args)
{
	int amount; //amount of cards to lose
	try //check the input
	{
		amount=args.at(0);
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		int handIndex = (std::uniform_int_distribution<int>(0, static_cast<int>(_cardHand.size())))(_engine);
		cardHandToGraveyard(handIndex);
	}
}

void Player::reviveGraveyardCard(const EffectParamsCollection& args)
{
	int binIndex; //what card to revive
	try //check the input
	{
		binIndex=args.at(0);
		_cardGraveyard.at(binIndex);
	}
	catch (std::out_of_range&)
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
	Card* myCard;

	try //check the input
	{
		myCardIndex = args.at(0);
		myCard = _cardHand.at(myCardIndex);
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	Card* hisCard =  _opponent->cardExchangeFromHand(myCard);

	if (hisCard == nullptr)
		packet << TransferType::FAILURE;
	else
	{
		cardExchangeFromHand(hisCard, myCardIndex);
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
	std::vector<EffectParamsCollection> effects = usedCard->getEffects();
	for (unsigned i=0; i<effects.size(); i++) //for each effect of the card
	{
		applyEffect(usedCard, effects.at(i)); //apply it
	}
}

void Player::setTeamConstraint(const Card* /* usedCard */, const EffectParamsCollection& args)
{
	int constraintID; //constraint to set
	int value; //value to give to it
	int turns; //for how many turns
	int casterOptions; //whether the constraint depends on its caster being alive
	try //check the input
	{
		constraintID=args.at(0);
		value=args.at(1);
		turns=args.at(2);
		casterOptions=args.at(3);
		if (constraintID<0 or constraintID>=C_CONSTRAINTS_COUNT or turns<0)
			throw std::out_of_range("");
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}

	switch (casterOptions)
	{
		case IF_CASTER_ALIVE:
			_constraints.setConstraint(constraintID, value, turns, dynamic_cast<const Creature*>(getLastCaster()));
			break;
		default:
			_constraints.setConstraint(constraintID, value, turns);
			break;
	}
}

void Player::cardDeckToHand(int amount)
{
	while(not _cardDeck.empty() and amount>0)
	{
		amount--;
		_cardHand.push_back(_cardDeck.top());
		_cardDeck.pop();
	}
	logHandState();
	_opponent->logOpponentHandState();
	logCurrentDeck();
}

void Player::cardHandToBoard(int handIndex)
{
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBoard.push_back(dynamic_cast<Creature*>(_cardHand.at(handIndex)));
	_cardBoard.back()->movedToBoard();
	_cardHand.erase(handIt);
	logHandState();
	_opponent->logOpponentHandState();
	logBoardState();
	_opponent->logOpponentBoardState();
}

void Player::cardHandToGraveyard(int handIndex)
{
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardGraveyard.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIt);
	logHandState();
	_opponent->logOpponentHandState();
	logGraveyardState();
}

void Player::cardBoardToGraveyard(int boardIndex)
{
	const auto& boardIt = std::find(_cardBoard.begin(), _cardBoard.end(), _cardBoard[boardIndex]);
	_cardBoard.at(boardIndex)->removedFromBoard();
	_cardGraveyard.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIt);
	logBoardState();
	_opponent->logOpponentBoardState();
	logGraveyardState();
}

void Player::cardGraveyardToHand(int binIndex)
{
	const auto& binIt = std::find(_cardGraveyard.begin(), _cardGraveyard.end(), _cardGraveyard[binIndex]);
	_cardHand.push_back(_cardGraveyard.at(binIndex));
	_cardGraveyard.erase(binIt);
	logGraveyardState();
	logHandState();
	_opponent->logOpponentHandState();
}

void Player::cardAddToHand(Card* givenCard)
{
	if (givenCard!=nullptr)
	{
		_cardHand.push_back(givenCard);
		logHandState();
		_opponent->logOpponentHandState();
	}
}

Card* Player::cardRemoveFromHand()
{
	if (_cardHand.empty())
		return nullptr;
	int handIndex = (std::uniform_int_distribution<int>(0, static_cast<int>(_cardHand.size())))(_engine);
	Card* stolenCard = _cardHand[handIndex];
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardHand.erase(handIt);
	logHandState();
	_opponent->logOpponentHandState();
	return stolenCard;
}

Card* Player::cardExchangeFromHand(Card* givenCard)
{
	int handIndex = (std::uniform_int_distribution<int>(0, static_cast<int>(_cardHand.size())))(_engine);
	return cardExchangeFromHand(givenCard, handIndex);
}

Card* Player::cardExchangeFromHand(Card* givenCard, int handIndex)
{
	if (_cardHand.empty())
		return nullptr;
	Card* stolen = _cardHand[handIndex];
	_cardHand.at(handIndex) = givenCard;
	logHandState();
	_opponent->logOpponentHandState();
	return stolen;
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
void Player::logIdsFromVector(TransferType type, const std::vector<CardType *>& vect)
{
	std::vector<sf::Uint32> cardIds{static_cast<sf::Uint32>(vect.size())};
	for(typename std::vector<CardType *>::size_type i{0}; i < vect.size(); ++i)
		cardIds[i] = vect[i]->getID();

	_pendingBoardChanges << type << cardIds;
}

void Player::logCardDataFromVector(TransferType type, const std::vector<Card*>& vect)
{
	std::vector<CardData> cards;
	for(auto i=0U; i < vect.size(); ++i)
	{
		CardData data;
		data.id = vect.at(i)->getID();
		cards.push_back(data);
	}
	_pendingBoardChanges << type << cards;
}

void Player::logBoardCreatureDataFromVector(TransferType type, const std::vector<Creature*>& vect)
{
	std::vector<BoardCreatureData> boardCreatures;
	for (auto i=0U; i<vect.size(); i++)
	{
		BoardCreatureData data;
		Creature& creat = *vect.at(i);
		data.id 	= creat.getID();
		data.attack = creat.getAttack();
		data.health = creat.getHealth();
		data.shield = creat.getShield();
		int shieldType = creat.getShieldType();
		switch (shieldType)
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
std::vector<int>&& Player::askUserToSelectCards(const std::vector<CardToSelect>& selection)
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
	// covnert the sf::Uint32 received on the network by implementation-defined integers
	std::vector<int> ret(selection.size());
	for(auto i{0U}; i < selection.size(); ++i)
		ret[i] = static_cast<int>(indices[i]);
	return std::move(ret);
}

std::vector<int>&& Player::getRandomBoardIndexes(const std::vector<CardToSelect>& selection)
{
	std::vector<int> indices(selection.size());
	for (std::size_t i{0}; i < selection.size(); i++)
	{
		switch (selection.at(i))
		{
			case CardToSelect::SELF_BOARD:
				indices.push_back(std::uniform_int_distribution<int>(0, static_cast<int>(_cardBoard.size())-1)(_engine));
				break;
			case CardToSelect::SELF_HAND:
				indices.push_back(std::uniform_int_distribution<int>(0, static_cast<int>(_cardHand.size())-1)(_engine));
				break;
			case CardToSelect::OPPO_BOARD:
				indices.push_back(std::uniform_int_distribution<int>(0, static_cast<int>(_opponent->_cardBoard.size())-1)(_engine));
				break;
			case CardToSelect::OPPO_HAND:
				indices.push_back(std::uniform_int_distribution<int>(0, static_cast<int>(_opponent->_cardHand.size())-1)(_engine));
				break;
		}
	}
	return std::move(indices);
}

void Player::sendValueToClient(TransferType transferType)
{
	sf::Packet packet;
	packet << transferType;
	_socketToClient.send(packet);
}

Player::~Player()
{
	// free the memory (all of the card pointers allocated)
	for(auto& card: _cardBoard)
		delete card;
	for(auto& card: _cardGraveyard)
		delete card;
	for(auto& card: _cardHand)
		delete card;
	while(not _cardDeck.empty())
	{
		delete _cardDeck.top();
		_cardDeck.pop();
	}
}

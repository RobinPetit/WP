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
std::array<std::function<void(Player&, EffectArgs)>, P_EFFECTS_COUNT> Player::_effectMethods =
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

Player::Player(GameThread& gameThread, ServerDatabase& database, userId id, Player& opponent):
	_gameThread(gameThread),
	_database(database),
	_opponent(opponent),
	_id(id),
	_isActive(false)
{
	// Input sockets are non-blocking, this is easier since the sockets of the
	// two players are separated in the Player instances. So that as soon as a
	// packet is received, the packet is handled, rather than waiting that the
	// other Player instance receives a packet from the other client.
	_socketToClient.setBlocking(false);
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
	for(std::size_t i{0}; i < Deck::size; ++i) // \TODO use database instead
	{
		const cardId card{newDeck.getCard(i)};
		// TODO: test use of database
		loadedCards[i].reset(_database.getCard(card));
	}

	// shuffle the deck of cards
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

void Player::setUpGame(bool isActivePlayer)
{
	printVerbose(std::string("Player::setUpGame(") + (isActivePlayer ? "true" : "false") + ")");

	// init Player's data
	cardDeckToHand(_initialSupplementOfCards);  // start with a few cards in hand
	_turnData = _emptyTurnData;
	_turnsSinceEmptyDeck = 0;
	_energy = _energyInit = 0;
	_health = _healthInit = 20;

	// log & send
	logEverything();
	_socketToClient.send(_pendingBoardChanges);
	_pendingBoardChanges.clear();

	// send GAME_STARTING packet
	sf::Packet packet;
	packet << TransferType::GAME_STARTING << (isActivePlayer ? TransferType::GAME_PLAYER_ENTER_TURN : TransferType::GAME_PLAYER_LEAVE_TURN);
	_socketToClient.send(packet);

	_isActive.store(isActivePlayer); // Player has become active/passive
}

void Player::enterTurn(int turn)
{
	printVerbose(std::string("Player::enterTurn(") + std::to_string(turn) + ")");

	_isActive.store(true); // Player has become active

	_turnData = _emptyTurnData;  // Reset the turn data
	_turnData.turnCount = turn;  // Store turn number

	// player loses if he spends 10 turns with empty deck
	if(_cardDeck.empty() and ++_turnsSinceEmptyDeck == _maximumAmountOfTurnsWithEmptyDeck)
		finishGame(false, EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK);

	// Player's turn-based constraints
	cardDeckToHand(_constraints.getConstraint(PC_TURN_CARDS_PICKED));
	resetEnergy({_constraints.getConstraint(PC_TURN_ENERGY_INIT_CHANGE)});
	changeEnergy({_constraints.getConstraint(PC_TURN_ENERGY_CHANGE)});
	changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE)});
	if(_cardDeck.empty())
		changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE_DECK_EMPTY)});

	// his Creature's turn-based constraints
	for(unsigned i = 0; i < _cardBoard.size(); i++)
		_cardBoard.at(i)->enterTurn();

	// log all data, since most things change at the beginning of the turn
	logEverything(); // also clears previous logs
	_opponent.logEverything();
}

void Player::leaveTurn()
{
	_isActive.store(false); // Player is no longer active

	// Time out Player's constraints
	_constraints.timeOutConstraints();
	_teamConstraints.timeOutConstraints();

	// Time out his Creature's constraints
	for(unsigned i = 0; i < _cardBoard.size(); i++)
		_cardBoard.at(i)->leaveTurn();
}

void Player::finishGame(bool hasWon, EndGame::Cause cause)
{
	_gameThread.endGame(hasWon ? getId() : _opponent.getId(), cause);
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
			// \TODO: add TransferType::GAME_QUIT and send if from client when game is quit ? (different from disconnected player)
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
	_opponent._lastCasterCard = usedCard;

	(this->*(usedCard->isCreature() ? &Player::useCreature : &Player::useSpell))(handIndex, usedCard);
	logHandState();
	_opponent.logOpponentHandState();
	logCurrentEnergy();

	logBoardState();
	logOpponentBoardState();
	_opponent.logBoardState();
	_opponent.logOpponentBoardState();
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

	exploitCardEffects(usedCard);
	cardHandToBoard(handIndex);
	sendValueToClient(TransferType::ACKNOWLEDGE);
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

	// if one of the required effects could not have been applied, then
	// the spell is not usable for now and so don't use the card
	if(exploitCardEffects(usedCard))
	{
		cardHandToGraveyard(handIndex);
		sendValueToClient(TransferType::ACKNOWLEDGE);
	}
	// no need to send FAILURE: exploitCardEffects call applyEffect
	// which sends FAILURE the function returns false.
}

void Player::attackWithCreature(int attackerIndex, int victimIndex)
{
	Creature* attacker;
	Creature* victim;
	try //check the input
	{
		attacker = _cardBoard.at(attackerIndex).get();
		if(victimIndex >= 0)
			victim = _opponent._cardBoard.at(victimIndex).get();
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
		_opponent.changeHealth({-attacker->getAttack()});
		logOpponentHealth();
		_opponent.logCurrentHealth();
	}
	else
	{
		attacker->makeAttack(*victim);
		victim->makeAttack(*attacker);
		logOpponentBoardState();
		logBoardState();  // If an attack is returned to the attacker, the board changes
		_opponent.logOpponentBoardState();
		_opponent.logBoardState();
	}
	logCurrentEnergy();
	sendValueToClient(TransferType::ACKNOWLEDGE);
}

void Player::endTurn()
{
	_gameThread.swapTurns();
}

/*------------------------------ EFFECTS INTERFACE */
bool Player::applyEffect(Card* usedCard, EffectArgs effect)
{
	int subject;  // who the effect applies to
	try  // check the input
	{
		subject = effect.getArg();
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("applyEffect error with cards arguments");
	}

	_lastCasterCard = usedCard;  // remember last used card
	_opponent._lastCasterCard = usedCard; // same for opponent
	bool ret{true};

	switch(subject)
	{
		case PLAYER_SELF:  //passive player
			askUserToSelectCards({});
			applyEffectToSelf(effect);
			break;

		case PLAYER_OPPO:  //active player
			askUserToSelectCards({});
			_opponent.applyEffectToSelf(effect);
			break;

		case CREATURE_SELF_THIS:  //active player's creature that was used
		{
			askUserToSelectCards({});
			Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
			applyEffectToCreature(usedCreature, effect);
		}
			break;

		case CREATURE_SELF_INDX:  //active player's creature at given index
			if(not _cardBoard.empty())
				applyEffectToCreature(effect, askUserToSelectCards({CardToSelect::SELF_BOARD}));
			else
			{
				sendValueToClient(TransferType::FAILURE);
				ret = false;
			}
			break;

		case CREATURE_SELF_RAND:  //active player's creature at random index
			askUserToSelectCards({});
			if(not _cardBoard.empty())
				applyEffectToCreature(effect, getRandomBoardIndexes({CardToSelect::SELF_BOARD}));
			else
			{
				sendValueToClient(TransferType::FAILURE);
				ret = false;
			}
			break;

		case CREATURE_SELF_TEAM:  //active player's team of creatures
			askUserToSelectCards({});
			applyEffectToCreatureTeam(effect);
			break;

		case CREATURE_OPPO_INDX:	//passive player's creature at given index
			if(not _opponent._cardBoard.empty())
				_opponent.applyEffectToCreature(effect, askUserToSelectCards({CardToSelect::OPPO_BOARD}));
			else
			{
				sendValueToClient(TransferType::FAILURE);
				ret = false;
			}
			break;

		case CREATURE_OPPO_RAND:	//passive player's creature at random index
			askUserToSelectCards({});
			if(not _opponent._cardBoard.empty())
				_opponent.applyEffectToCreature(effect, getRandomBoardIndexes({CardToSelect::OPPO_BOARD}));
			else
			{
				sendValueToClient(TransferType::FAILURE);
				ret = false;
			}
			break;

		case CREATURE_OPPO_TEAM:	//passive player's team of creatures
			askUserToSelectCards({});
			_opponent.applyEffectToCreatureTeam(effect);
			break;

		default:
			throw std::runtime_error("Effect subject not valid");
	}
	return ret;
}

void Player::applyEffectToSelf(EffectArgs effect)
{
	int method = effect.getArg();
	_effectMethods.at(method)(*this, effect);  //call method on self
}

void Player::applyEffectToCreature(Creature* casterAndSubject, EffectArgs effect)
{
	casterAndSubject->applyEffectToSelf(effect); //call method on effect subject (same as caster)
}

void Player::applyEffectToCreature(EffectArgs effect, const std::vector<int>& boardIndexes)
{
	_cardBoard.at(boardIndexes.at(0))->applyEffectToSelf(effect);
}

void Player::applyEffectToCreatureTeam(EffectArgs effect)
{
	//If the effect consists in setting a constraint
	if(effect.peekArg() == CE_SET_CONSTRAINT)
	{
		effect.getArg();
		setTeamConstraint(effect); //set a team constraint instead of individual ones
	}
	else //other effects just get applied to each creature individually
		for(auto& card : _cardBoard)
			card->applyEffectToSelf(effect);
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

void Player::printVerbose(std::string message)
{
    _gameThread.printVerbose("player " + std::to_string(getId()) + " - "+ message);
}

/*------------------------------ EFFECTS (PRIVATE) */
void Player::setConstraint(EffectArgs effect)
{
	printVerbose("Player::setConstraint(" + effect.toString() + ")");
	int constraintId; //constraint to set
	int value; //value to give to it
	int turns; //for how many turns
	int casterOptions; //whether the constraint depends on its caster being alive
	try //check the input
	{
		constraintId = effect.getArg();
		value = effect.getArg();
		turns = effect.getArg();
		casterOptions = effect.getArg();
		if (constraintId<0 or constraintId>=P_CONSTRAINTS_COUNT or turns<0)
			throw std::out_of_range("");
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("Player::setConstraint error with cards arguments");
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

void Player::pickDeckCards(EffectArgs effect)
{
	printVerbose("Player::pickDeckCards(" + effect.toString() + ")");
	int amount;//amount of cards
	try //check the input
	{
		amount = effect.getArg();
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("picDeckCards error with cards arguments");
	}
	cardDeckToHand(amount);
}

void Player::loseHandCards(EffectArgs effect)
{
	printVerbose("Player::loseHandCards(" + effect.toString() + ")");
	try  //check the input
	{
		for(int amount{effect.peekArg()}; not _cardHand.empty() and amount > 0; amount--)
			cardHandToGraveyard(getRandomIndex(_cardHand));
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("loseHandCards error with cards arguments");
	}
}

void Player::reviveGraveyardCard(EffectArgs effect)
{
	printVerbose("Player::reviveGraveyardCard()");
	int binIndex;  //what card to revive
	try  //check the input
	{
		binIndex = effect.peekArg();
		_cardGraveyard.at(binIndex);
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("receiveGraveyardCard error with cards arguments");
	}
	cardGraveyardToHand(binIndex);
}

void Player::stealHandCard(EffectArgs /* effect */)
{
	printVerbose("Player::stealHandCard()");
	//no arguments
	cardAddToHand(_opponent.cardRemoveFromHand());
}

/// \network sends to user one of the following:
///	 + FAILURE if opponent has no card in his hand
///	 + ACKNOWLEDGE if card has been swapped
void Player::exchgHandCard(EffectArgs /* effect */)
{
	printVerbose("Player::exchgHandCard()");
	int myCardIndex; //card to exchange
	sf::Packet packet;
	std::unique_ptr<Card> myCard;

	try //check the input
	{
		myCardIndex = getRandomIndex(_cardHand);
		myCard = std::move(_cardHand.at(myCardIndex));
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("exchgCard error with cards arguments");
	}

	std::unique_ptr<Card> hisCard(_opponent.cardExchangeFromHand(std::move(myCard)));

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

void Player::resetEnergy(EffectArgs effect)
{
	printVerbose("Player::resetEnergy(" + effect.toString() + ")");
	try //check the input
	{
		_energyInit += effect.getArg();
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("resetEnergy error with cards arguments");
	}
	if(_energyInit < 0)
		_energyInit = 0;
	else if(_energyInit > _maxEnergy)
		_energyInit = _maxEnergy;
	_energy = _energyInit;
	logCurrentEnergy();
}

void Player::changeEnergy(EffectArgs effect)
{
	printVerbose("Player::changeEnergy(" + effect.toString() + ")");
	try //check the input
	{
		_energy += effect.getArg();
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("changeEnergy error with cards arguments");
	}
	if(_energy < 0)
		_energy = 0;
	else if(_energy > _maxEnergy)
		_energy = _maxEnergy;
	logCurrentEnergy();
}

void Player::changeHealth(EffectArgs effect)
{
	printVerbose("Player::changeHealth(" + effect.toString() + ")");

	int points;
	try //check the input
	{
		points = effect.getArg();
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("changeHealth error with cards arguments");
	}

	_health += points;
	if(_health <= 0)
	{
		_health = 0;
		finishGame(false, EndGame::Cause::OUT_OF_HEALTH);
	}
	else if(_health > _maxHealth)
	{
		_health = _maxHealth;
	}
	logCurrentHealth();
	_opponent.logOpponentHealth();
}


/*--------------------------- PRIVATE */

bool Player::exploitCardEffects(Card* usedCard)
{
	// Maybe I should have use multiple inheritance to avoid this
	const std::vector<EffectParamsCollection>& effects(
		usedCard->isSpell()
			? static_cast<Spell *>(usedCard)->getEffects()
			: static_cast<Creature *>(usedCard)->getEffects()
	);

	for(const auto& effect: effects) //for each effect of the card
		if(not applyEffect(usedCard, effect)) //apply it
			return false;
	return true;
}

void Player::setTeamConstraint(EffectArgs effect)
{
	int constraintId;  // constraint to set
	int value;  // value to give to it
	int turns;  // for how many turns
	int casterOptions;  // whether the constraint depends on its caster being alive
	try  // check the input
	{
		constraintId = effect.getArg();
		value = effect.getArg();
		turns = effect.getArg();
		casterOptions = effect.getArg();
		if(constraintId < 0 or constraintId >= C_CONSTRAINTS_COUNT or turns < 0)
			throw std::out_of_range("");
	}
	catch(std::out_of_range&)
	{
		throw std::runtime_error("setTeamConstraint error with cards arguments");
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
	_opponent.logOpponentHandState();
	logCurrentDeck();
}

void Player::cardHandToBoard(int handIndex)
{
	// Release the ownership of the hand, cast to a Creature pointer and give it to the board
	_cardBoard.push_back(std::unique_ptr<Creature>(dynamic_cast<Creature*>(_cardHand.at(handIndex).release())));
	_cardBoard.back()->moveToBoard();
	_cardHand.erase(_cardHand.begin() + handIndex);
	logHandState();
	_opponent.logOpponentHandState();
	logBoardState();
	_opponent.logOpponentBoardState();
}

void Player::cardHandToGraveyard(int handIndex)
{
	_cardGraveyard.push_back(std::move(_cardHand.at(handIndex)));
	_cardHand.erase(_cardHand.begin() + handIndex);
	logHandState();
	_opponent.logOpponentHandState();
	logGraveyardState();
}

void Player::cardBoardToGraveyard(int boardIndex)
{
	_cardBoard.at(boardIndex)->removeFromBoard();
	// Release the ownership of the board, cast to a Card pointer and give it to the graveyard
	_cardGraveyard.push_back(std::unique_ptr<Card>(static_cast<Card*>(_cardBoard.at(boardIndex).release())));
	_cardBoard.erase(_cardBoard.begin() + boardIndex);
	logBoardState();
	_opponent.logOpponentBoardState();
	logGraveyardState();
}

void Player::cardBoardToGraveyard(const Creature *card)
{
	const auto& lambdaCheck{[card](const auto& it){return it.get() == card;}};
	const auto& cardIterator{std::find_if(_cardBoard.begin(), _cardBoard.end(), lambdaCheck)};
	cardBoardToGraveyard(static_cast<int>(cardIterator - _cardBoard.begin()));
}

void Player::cardGraveyardToHand(int binIndex)
{
	_cardHand.push_back(std::move(_cardGraveyard.at(binIndex)));
	_cardGraveyard.erase(_cardGraveyard.begin() + binIndex);
	logGraveyardState();
	logHandState();
	_opponent.logOpponentHandState();
}

void Player::cardAddToHand(std::unique_ptr<Card> givenCard)
{
	if(givenCard != nullptr)
	{
		_cardHand.push_back(std::move(givenCard));
		logHandState();
		_opponent.logOpponentHandState();
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
	_opponent.logOpponentHandState();
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
	_opponent.logOpponentHandState();
	return std::move(givenCard);
}


/*--------------------------- LOGGERS */

void Player::logEverything()
{
	//since everything will be logged, we can clear previous logs
	_pendingBoardChanges.clear();

	logCurrentEnergy();
	logCurrentHealth();
	logOpponentHealth();
	logCurrentDeck();

	logHandState();
	logOpponentHandState();
	logBoardState();
	logOpponentBoardState();
	logGraveyardState();
}

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
	try
	{
		_pendingBoardChanges << TransferType::GAME_OPPONENT_HEALTH_UPDATED << static_cast<sf::Uint32>(_opponent.getHealth());
	}
	catch (...)  // In case _opponent is not initialized yet
	{
        _pendingBoardChanges << TransferType::GAME_OPPONENT_HEALTH_UPDATED << static_cast<sf::Uint32>(_healthInit);
	}
}

void Player::logCurrentDeck()
{
	_pendingBoardChanges << TransferType::GAME_DECK_UPDATED << static_cast<sf::Uint32>(_cardDeck.size());
}

void Player::logHandState()
{
	logCardDataFromVector(TransferType::GAME_HAND_UPDATED, _cardHand);
}

void Player::logOpponentHandState()
{
	try
	{
		_pendingBoardChanges << TransferType::GAME_OPPONENT_HAND_UPDATED << static_cast<sf::Uint32>(_opponent.getHandSize());
	}
	catch (...)  // In case _opponent is not initialized yet
	{
		_pendingBoardChanges << TransferType::GAME_OPPONENT_HAND_UPDATED << static_cast<sf::Uint32>(_initialSupplementOfCards+1);
	}
}

void Player::logBoardState()
{
	logBoardCreatureDataFromVector(TransferType::GAME_BOARD_UPDATED, _cardBoard);
}

void Player::logOpponentBoardState()
{
	logBoardCreatureDataFromVector(TransferType::GAME_OPPONENT_BOARD_UPDATED, _opponent.getBoard());
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
	for(const auto& creature : vect)
		boardCreatures.push_back(static_cast<BoardCreatureData>(*creature));
	// std::vector transmission in packet is defined in common/sockets/PacketOverload.hpp
	_pendingBoardChanges << type << boardCreatures;
}


// TODO: handle the case where the user doesn't give and his turn finishes
std::vector<int> Player::askUserToSelectCards(const std::vector<CardToSelect>& selection)
{
	sf::Packet packet;
	packet << TransferType::ACKNOWLEDGE << selection;
	_socketToClient.send(packet);
	std::vector<sf::Uint32> indices;
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
				indices[i] = getRandomIndex(_opponent._cardBoard);
				break;

			case CardToSelect::OPPO_HAND:
				indices[i] = getRandomIndex(_opponent._cardHand);
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

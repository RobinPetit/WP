// WizardPoker headers
#include "server/Player.hpp"
#include "server/Board.hpp"
#include "server/Creature.hpp"
#include "common/sockets/TransferType.hpp"
// std-C++ headers
#include <algorithm>
// SFML headers
#include <SFML/Network/Packet.hpp>

std::function<void(Player&, const EffectParamsCollection&)> Player::_effectMethods[P_EFFECTS_COUNT] =
{
	&Player::setConstraint,
	&Player::pickDeckCards,
	&Player::loseHandCards,
	&Player::reviveBinCard,

	&Player::stealHandCard,
	&Player::exchgHandCard,

	&Player::setEnergy,
	&Player::changeEnergy,
	&Player::changeHealth,
};

Player::Player(Player::ID id, sf::TcpSocket& socket):
	_id(id),
	_socketToClient(socket)
{
	//NETWORK: GREETINGS_USER
}

void Player::setOpponent(Player* opponent)
{
	_opponent = opponent;
}

Player::ID Player::getID()
{
	return _id;
}

/*------------------------------ BOARD INTERFACE */
void Player::beginGame(bool isActivePlayer)
{
    //if (isActivePlayer)
		//NETWORK: GAME_STARTED_ACTIVE
	//else
		//NETWORK: GAME_STARTED_INACTIVE

	//TODO: request Deck selection
}

void Player::enterTurn(int turn)
{
	_turnData = _emptyTurnData;  // Clear the turn data

	//Player's turn-based rules
	cardDeckToHand(_constraints.getConstraint(PC_TURN_CARDS_PICKED));
	setEnergy({_constraints.getConstraint(PC_TURN_ENERGY_INIT)});
	changeEnergy({_constraints.getConstraint(PC_TURN_ENERGY_CHANGE)});
	changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE)});
	if (_cardDeck.empty())
		changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE_DECK_EMPTY)});

	//Will call creature's turn-based rules
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->enterTurn();

	// Tell to player that its turn starts
	sf::Packet packet;
	packet << TransferType::GAME_PLAYER_ENTER_TURN;
	_socketToClient.send(packet);
}

void Player::leaveTurn()
{
	//Time out player constraints
	_constraints.timeOutConstraints();
	_teamConstraints.timeOutConstraints();

	//Time out player's creature's constraints
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->leaveTurn();
}

void Player::useCard(int handIndex)
{
	//TODO: verify that handIndex is not out_of_range

	if (_constraints.getConstraint(PC_TEMP_CARD_USE_LIMIT) == _turnData.cardsUsed)
	{
		//NETWORK: USE_CARDS_LIMIT
		return;
	}
	Card* usedCard = _cardHand.at(handIndex);

	//TODO: use typeinfo ?
	if (usedCard->isCreature())
		useCreature(handIndex, usedCard);
	else
		useSpell(handIndex, usedCard);
}

////////////////////// specialized card cases

void Player::useCreature(int handIndex, Card *& usedCard)
{
	sf::Packet response;
	if (_constraints.getConstraint(PC_TEMP_CREATURE_PLACING_LIMIT) == _turnData.creaturesPlaced)
		response << TransferType::SERVER_UNABLE_TO_PERFORM;
	else
	{
		_turnData.cardsUsed++;
		_turnData.creaturesPlaced++;
		cardHandToBoard(handIndex);
		exploitCardEffects(usedCard);
		response << TransferType::SERVER_ACKNOWLEDGEMENT;
	}
	_socketToClient.send(response);
}

void Player::useSpell(int handIndex, Card *& usedCard)
{
	sf::Packet response;
	if (_constraints.getConstraint(PC_TEMP_SPELL_CALL_LIMIT) == _turnData.spellCalls)
		response << TransferType::SERVER_UNABLE_TO_PERFORM;
	else
	{
		_turnData.cardsUsed++;
		_turnData.spellCalls++;
		cardHandToBin(handIndex);
		exploitCardEffects(usedCard);
		response << TransferType::SERVER_ACKNOWLEDGEMENT;
	}
	_socketToClient.send(response);
}

void Player::attackWithCreature(int attackerIndex, int victimIndex)
{
	sf::Packet response;
	if (_constraints.getConstraint(PC_TEMP_CREATURE_ATTACK_LIMIT) == _turnData.creatureAttacks)
		response << TransferType::SERVER_UNABLE_TO_PERFORM;
	else
	{
		Creature* attacker = _cardBoard.at(attackerIndex);
		if (victimIndex<0)
			_opponent->applyEffect(attacker, PE_CHANGE_HEALTH, {attacker->getAttack()}); //no forced attacks on opponent
		else
			attacker->makeAttack(*_opponent->_cardBoard.at(victimIndex));
		response << TransferType::SERVER_ACKNOWLEDGEMENT;
	}
	_socketToClient.send(response);
}

/*------------------------------ APPLYING EFFECTS */
void Player::applyEffect(const Card* usedCard, int method, const EffectParamsCollection& effectArgs)
{
	_lastCasterCard = usedCard;
	_effectMethods[method](*this, effectArgs);
}

void Player::applyEffectToCreature(Creature* casterAndSubject, int method, const EffectParamsCollection& effectArgs)
{
	_lastCasterCard = casterAndSubject;
	casterAndSubject->applyEffect(method, effectArgs);
}

void Player::applyEffectToCreature(const Card* usedCard, int boardIndex, int method, const EffectParamsCollection& effectArgs)
{
	_lastCasterCard = usedCard;
	//If no subject was selected
	if (boardIndex<0)
		boardIndex = rand() % _cardBoard.size();

	Creature* subject = _cardBoard.at(boardIndex);
	subject->applyEffect(method, effectArgs);
}

void Player::applyEffectToCreatures(const Card* usedCard, int method, const EffectParamsCollection& effectArgs)
{
	_lastCasterCard = usedCard;
	//If the effect consists in setting a constraint
	if (method==CE_SET_CONSTRAINT)
		setTeamConstraint(usedCard, effectArgs); //We set a team constraint instead of individual ones
	else
	{
		for (unsigned i=0; i<_cardBoard.size(); i++)
		{
			applyEffectToCreature(usedCard, i, method, effectArgs);
		}
	}
}

int Player::getCreatureConstraint(const Creature& subject, int constraintID)
{
	int creatureValue = subject.getPersonalConstraint(constraintID);
	return _teamConstraints.getOverallConstraint(constraintID, creatureValue);
}

const Card* Player::getLastCaster()
{
    return _lastCasterCard;
}

/*--------------------------- EFFECTS */
void Player::setConstraint(const EffectParamsCollection& args)
{
	int constraintID = args.at(0);
	int value = args.at(1);
	int turns = args.at(2);
	_constraints.setConstraint(constraintID, value, turns);
}

void Player::pickDeckCards(const EffectParamsCollection& args)
{
	cardDeckToHand(args.at(0));
}

void Player::loseHandCards(const EffectParamsCollection& args)
{
	int amount = args.at(1);
	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		int handIndex = (std::uniform_int_distribution<int>(0, _cardHand.size()))(_engine);
		cardHandToBin(handIndex);
	}
}

void Player::reviveBinCard(const EffectParamsCollection& args)
{
	int binIndex = args.at(0);
	cardBinToHand(binIndex);
}

void Player::stealHandCard(const EffectParamsCollection&)
{
	cardAddToHand(_opponent->cardRemoveFromHand());
}

void Player::exchgHandCard(const EffectParamsCollection& args)
{
	//TODO: check index
	int myCardIndex = args.at(0);
	Card* myCard = _cardHand.at(myCardIndex);
	Card* hisCard =  _opponent->cardExchangeFromHand(myCard);

	if (hisCard == nullptr)
	{
		//NETWORK: COULD_NOT_EXCHANGE
	}
	else
	{
		cardExchangeFromHand(hisCard, myCardIndex);
	}
}

void Player::setEnergy(const EffectParamsCollection& args)
{
	int points = args.at(0);
	_energy = points;
	if (_energy<0)
		_energy=0;
}

void Player::changeEnergy(const EffectParamsCollection& args)
{
	int points = args.at(0);
	_energy+=points;
	if (_energy<0)
		_energy=0;
	//NETWORK: ENERGY_CHANGED
}

void Player::changeHealth(const EffectParamsCollection& args)
{
	int points = args.at(0);
	_health+=points;
	if (_health<=0)
	{
		_health=0;
		//NETWORK: NO_HEALTH_CHANGED
		//call die()
	}
}


/*--------------------------- PRIVATE */
void Player::exploitCardEffects(Card* usedCard)
{
	std::vector<EffectParamsCollection> effects = usedCard->getEffects();
	for (unsigned i=0; i<effects.size(); i++) //for each effect of the card
	{
		_board->applyEffect(usedCard, effects.at(i)); //apply it
	}
}

void Player::cardDeckToHand(int amount)
{
	if(not _cardDeck.empty() and amount>0)
	{
		amount--;
		_cardHand.push_back(_cardDeck.top());
		_cardDeck.pop();
		//NETWORK: DECK_EMPTY
	}
	//NETWORK: DECK_CHANGED
	//NETWORK: HAND_CHANGED
}

void Player::cardHandToBoard(int handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBoard.push_back(dynamic_cast<Creature*>(_cardHand.at(handIndex)));
	_cardBoard.back()->movedToBoard();
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: BOARD_CHANGED
}

void Player::cardHandToBin(int handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBin.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: BIN_CHANGED
}

void Player::cardBoardToBin(int boardIndex)
{
	const auto& boardIt = std::find(_cardBoard.begin(), _cardBoard.end(), _cardBoard[boardIndex]);
	_cardBoard.at(boardIndex)->removedFromBoard();
	_cardBin.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIt);
	//NETWORK: BOARD_CHANGED
	//NETWORK: BIN_CHANGED
}

void Player::cardBinToHand(int binIndex)
{
	const auto& binIt = std::find(_cardBin.begin(), _cardBin.end(), _cardBin[binIndex]);
	_cardHand.push_back(_cardBin.at(binIndex));
	_cardBin.erase(binIt);
	//NETWORK: BIN_CHANGED
	//NETWORK: HAND_CHANGED
}

void Player::cardAddToHand(Card* givenCard)
{
    _cardHand.push_back(givenCard);
    //NETWORK: HAND_CHANGED
    //NETWORK: CARD_WON
}

Card* Player::cardRemoveFromHand()
{
	if (_cardHand.empty())
		return nullptr;
	int handIndex = (std::uniform_int_distribution<int>(0, _cardHand.size()))(_engine);
	Card* stolenCard = _cardHand[handIndex];
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: CARD_STOLEN
	return stolenCard;
}

Card* Player::cardExchangeFromHand(Card* givenCard)
{
	int handIndex = (std::uniform_int_distribution<int>(0, _cardHand.size()))(_engine);
	return cardExchangeFromHand(givenCard, handIndex);
}

Card* Player::cardExchangeFromHand(Card* givenCard, int handIndex)
{
	if (_cardHand.empty())
		return nullptr;
	Card* stolen = _cardHand[handIndex];
	_cardHand.at(handIndex) = givenCard;
	//NETWORK: HAND_CHANGED
	//NETWORK: CARD_EXCHANGED
	return stolen;
}

void Player::setTeamConstraint(const Card* usedCard, const EffectParamsCollection& effectArgs)
{
    int constraintID = effectArgs.front();
    int value = effectArgs.at(1);
    int turns = effectArgs.at(2);
	_teamConstraints.setConstraint(constraintID, value, turns, dynamic_cast<const Creature*>(usedCard));
}

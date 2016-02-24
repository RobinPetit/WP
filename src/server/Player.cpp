// WizardPoker headers
#include "server/Player.hpp"
#include "server/Board.hpp"
#include "server/Creature.hpp"
#include "common/sockets/TransferType.hpp"
// std-C++ headers
#include <algorithm>
// SFML headers
#include <SFML/Network/Packet.hpp>

/*------------------------------ CONSTRUCTOR AND INIT */
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

Player::Player(Player::ID id, sf::TcpSocket& socket, sf::TcpSocket& specialSocket):
	_id(id),
	_socketToClient(socket),
	_specialSocketToClient(specialSocket)
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
	_turnData = _emptyTurnData;  // Reset the turn data

	//Player's turn-based constraints
	cardDeckToHand(_constraints.getConstraint(PC_TURN_CARDS_PICKED));
	setEnergy({_constraints.getConstraint(PC_TURN_ENERGY_INIT)});
	changeEnergy({_constraints.getConstraint(PC_TURN_ENERGY_CHANGE)});
	changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE)});
	if (_cardDeck.empty())
		changeHealth({_constraints.getConstraint(PC_TURN_HEALTH_CHANGE_DECK_EMPTY)});

	//Will call creature's turn-based constraints
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

/// \network sends to client one of the following:
///	 + SERVER_ACKNOWLEDGEMENT if the card was successfully used
///	 + GAME_CARD_LIMIT_TURN_REACHED if the user cannot play cards for this turn
///	 + SERVER_UNABLE_TO_PERFORM  if the specialized type of the card (spell/creature) cannot be played anymore for this turn
void Player::useCard(int handIndex)
{
	try //check the input
	{
		_cardHand.at(myCardIndex);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}

	if (_constraints.getConstraint(PC_TEMP_CARD_USE_LIMIT) == _turnData.cardsUsed)
	{
		sf::Packet packet;
		packet << TransferType::GAME_CARD_LIMIT_TURN_REACHED;
		_socketToClient.send(packet);
		return;
	}
	Card* usedCard = _cardHand.at(handIndex);

	//TODO: use typeinfo ?
	(*this.*(usedCard->isCreature() ? &Player::useCreature : &Player::useSpell))(handIndex, usedCard);
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

	try //check the input
	{
		_cardHand.at(attackerIndex);
		_opponent->_cardHand.at(victimIndex);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}
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

/*------------------------------ EFFECTS INTERFACE */
void Player::applyEffect(const Card* usedCard, EffectParamsCollection effectArgs)
{
	_lastCasterCard = usedCard; //remember last used card

	int method = effectArgs.front(); //what method is used
	effectArgs.erase(effectArgs.begin());

	_effectMethods[method](*this, effectArgs); //call method on self
}

void Player::applyEffectToCreature(Creature* casterAndSubject, EffectParamsCollection effectArgs)
{
	_lastCasterCard = casterAndSubject; //remember last used card
	casterAndSubject->applyEffect(effectArgs); //call method on effect subject (same as caster)
}

void Player::applyEffectToCreature(const Card* usedCard, int boardIndex, EffectParamsCollection effectArgs)
{
	_lastCasterCard = usedCard; //remember last used card

	//If no subject was selected, choose a random one
	if (boardIndex<0)
		boardIndex = rand() % _cardBoard.size();

	Creature* subject = _cardBoard.at(boardIndex);
	subject->applyEffect(effectArgs);
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
			applyEffectToCreature(usedCard, i, effectArgs);
}

/*------------------------------ GETTERS */
int Player::getCreatureConstraint(const Creature& subject, int constraintID)
{
	//returns the value respecting both the creature and the whole team's constraints
	int creatureValue = subject.getPersonalConstraint(constraintID);
	return _teamConstraints.getOverallConstraint(constraintID, creatureValue);
}

const Card* Player::getLastCaster()
{
	return _lastCasterCard;
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
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
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
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
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
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}

	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		int handIndex = (std::uniform_int_distribution<int>(0, _cardHand.size()))(_engine);
		cardHandToBin(handIndex);
	}
}

void Player::reviveBinCard(const EffectParamsCollection& args)
{
	int binIndex; //what card to revive
	try //check the input
	{
		binIndex=args.at(0);
		_cardBin.at(binIndex);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}
	cardBinToHand(binIndex);
}

void Player::stealHandCard(const EffectParamsCollection& args)
{
	//no arguments
	cardAddToHand(_opponent->cardRemoveFromHand());
}

/// \network sends to user one of the following:
///	 + SERVER_UNABLE_TO_PERFORM if opponent has no card in his hand
///	 + SERVER_ACKNOWLEDGEMENT if card has been swapped
void Player::exchgHandCard(const EffectParamsCollection& args)
{
	int myCardIndex; //card to exchange = args.at(0);
	Card* myCard;

	try //check the input
	{
		myCardIndex = args.at(0);
		myCard = _cardHand.at(myCardIndex);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}

	Card* hisCard =  _opponent->cardExchangeFromHand(myCard);

	sf::Packet packet;
	if (hisCard == nullptr)
		packet << TransferType::SERVER_UNABLE_TO_PERFORM;
	else
	{
		cardExchangeFromHand(hisCard, myCardIndex);
		packet << TransferType::SERVER_ACKNOWLEDGEMENT;
	}
	_socketToClient.send(packet);
}

void Player::setEnergy(const EffectParamsCollection& args)
{
	int points; //energy points to give
	try //check the input
	{
		points=args.at(0);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}
	_energy = points;
	if (_energy<0)
		_energy=0;
	sendCurrentEnergy();
}

void Player::changeEnergy(const EffectParamsCollection& args)
{
	int points; //points to add
	try //check the input
	{
		points=args.at(0);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}
	_energy+=points;
	if (_energy<0)
		_energy=0;
	sendCurrentEnergy();
}

void Player::changeHealth(const EffectParamsCollection& args)
{
	int points; //points to add
	try //check the input
	{
		points=args.at(0);
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}

	_health+=points;
	if (_health<=0)
	{
		_health=0;
		//NETWORK: NO_HEALTH_CHANGED
		//call die()
	}
	sendCurrentHealth();
}


/////////

void Player::sendCurrentEnergy()
{
	sf::Packet packet;
	// cast to be sure that the right amount of bits is sent and received
	packet << TransferType::GAME_PLAYER_ENERGY_UPDATED << static_cast<sf::Uint32>(_energy);
	_specialSocketToClient.send(packet);
}

void Player::sendCurrentHealth()
{
	sf::Packet packet;
	// cast to be sure that the right amount of bits is sent and received
	packet << TransferType::GAME_PLAYER_HEALTH_UPDATED << static_cast<sf::Uint32>(_health);
	_specialSocketToClient.send(packet);
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

void Player::setTeamConstraint(const Card* usedCard, const EffectParamsCollection& args)
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
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
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
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBoard.push_back(dynamic_cast<Creature*>(_cardHand.at(handIndex)));
	_cardBoard.back()->movedToBoard();
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: BOARD_CHANGED
}

void Player::cardHandToBin(int handIndex)
{
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

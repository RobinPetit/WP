// WizardPoker headers
#include "server/Player.hpp"
#include "server/Board.hpp"
#include "server/Creature.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
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

	&Player::resetEnergy,
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

const std::vector<Creature *>& Player::getBoard()
{
	return _cardBoard;
}

/*------------------------------ BOARD INTERFACE */
void Player::beginGame(bool isActivePlayer)
{
	//if (isActivePlayer)
		//NETWORK: GAME_STARTED_ACTIVE
	//else
		//NETWORK: GAME_STARTED_INACTIVE

	//NETWORK: request Deck selection from client
	loadCardDeck(0);
}

void Player::enterTurn(int turn)
{
	_turnData = _emptyTurnData;  // Reset the turn data

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
		_cardHand.at(handIndex);
	}
	catch (std::out_of_range)
	{
		sendValueToClient(_socketToClient, TransferType::FAILURE);
		return;
	}

	Card* usedCard = _cardHand.at(handIndex);
	//Check if we have enough energy to use this card
	if (usedCard->getEnergyCost() > _energy)
	{
		sendValueToClient(_socketToClient, TransferType::GAME_NOT_ENOUGH_ENERGY);
		return;
	}

	//Check if we have the right to put more cards (constraint)
	if (_constraints.getConstraint(PC_TEMP_CARD_USE_LIMIT) == _turnData.cardsUsed)
	{
		sendValueToClient(_socketToClient, TransferType::GAME_CARD_LIMIT_TURN_REACHED);
		return;
	}

	_energy -= usedCard->getEnergyCost();
	(*this.*(usedCard->isCreature() ? &Player::useCreature : &Player::useSpell))(handIndex, usedCard);
}

////////////////////// specialized card cases

void Player::useCreature(int handIndex, Card *& usedCard)
{
	sf::Packet response;
	if (_constraints.getConstraint(PC_TEMP_CREATURE_PLACING_LIMIT) == _turnData.creaturesPlaced)
		response << TransferType::FAILURE;
	else
	{
		_turnData.cardsUsed++;
		_turnData.creaturesPlaced++;
		cardHandToBoard(handIndex);
		exploitCardEffects(usedCard);
		response << TransferType::ACKNOWLEDGE;
	}
	_socketToClient.send(response);
}

void Player::useSpell(int handIndex, Card *& usedCard)
{
	sf::Packet response;
	if (_constraints.getConstraint(PC_TEMP_SPELL_CALL_LIMIT) == _turnData.spellCalls)
		response << TransferType::FAILURE;
	else
	{
		_turnData.cardsUsed++;
		_turnData.spellCalls++;
		cardHandToBin(handIndex);
		exploitCardEffects(usedCard);
		response << TransferType::ACKNOWLEDGE;
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
		sendValueToClient(_socketToClient, TransferType::FAILURE);
		return;
	}

	//Check if we have enough energy to use this card
	Creature* attacker = _cardBoard.at(attackerIndex);
	if (attacker->getEnergyCost() > _energy)
	{
		sendValueToClient(_socketToClient, TransferType::GAME_NOT_ENOUGH_ENERGY);
		return;
	}
	_energy -= attacker->getEnergyCost();

	if (_constraints.getConstraint(PC_TEMP_CREATURE_ATTACK_LIMIT) == _turnData.creatureAttacks)
		response << TransferType::FAILURE;
	else
	{
		if (victimIndex<0)
			_opponent->applyEffect(attacker, {PE_CHANGE_HEALTH, -attacker->getAttack()}); //no forced attacks on opponent
		else
			attacker->makeAttack(*_opponent->_cardBoard.at(victimIndex));
		response << TransferType::ACKNOWLEDGE;
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

void Player::applyEffectToCreature(const Card* usedCard, EffectParamsCollection effectArgs, int boardIndex)
{
	_lastCasterCard = usedCard; //remember last used card
	_cardBoard.at(boardIndex)->applyEffect(effectArgs);
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
			_cardBoard.at(i)->applyEffect(effectArgs);
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

int Player::getRandomBoardIndex()
{
	return std::uniform_int_distribution<int>(0, _cardBoard.size()-1)(_engine);
}

int Player::requestSelfBoardIndex()
{
	return 0; //NETWORK: REQUEST SELF BOARD INDEX
}

int Player::requestOppoBoardIndex()
{
	return 0; //NETWORK: REQUEST OPPO BOARD INDEX
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
		//SERVER: CARD_ERROR
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
		//SERVER: CARD_ERROR
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
		//SERVER: CARD_ERROR
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
		//SERVER: CARD_ERROR
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
	int myCardIndex; //card to exchange
	sf::Packet packet;
	Card* myCard;

	try //check the input
	{
		myCardIndex = args.at(0);
		myCard = _cardHand.at(myCardIndex);
	}
	catch (std::out_of_range)
	{
		//SERVER: CARD_ERROR
		return;
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
	int additionalPoints; //energy points to add to reset value
	try //check the input
	{
		additionalPoints=args.at(0);
	}
	catch (std::out_of_range)
	{
		//SERVER: CARD_ERROR
		return;
	}
	_energyInit += additionalPoints; //add points to initial amount of energy
	if (_energyInit<0)
		_energyInit=0;
	else if (_energy>_maxEnergy)
		_energyInit=_maxEnergy;

	_energy = _energyInit;
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
		//SERVER: CARD_ERROR
		return;
	}
	_energy+=points;
	if (_energy<0)
		_energy=0;
	else if (_energy>_maxEnergy)
		_energy=_maxEnergy;
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
		//SERVER: CARD_ERROR
		return;
	}

	_health+=points;
	if (_health<=0)
	{
		_health=0;
		//NETWORK: NO_HEALTH_CHANGED
		//call die()
	}
	else if (_health>_maxHealth) _health=_maxHealth;

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
void Player::loadCardDeck(int chosenDeck)
{
	//DATABASE: request chosen deck for Card Creation
	std::vector<Card* > loadedCards;
	for (int i=0; i<10; i++)
	{
		CreatureData creat = ALL_CREATURES[i];
		loadedCards.push_back(new Creature(i, creat.cost, creat.attack, creat.health, creat.shield, creat.shieldType, creat.effects));
	}
	for (int i=0; i<5; i++)
	{
		SpellData spell = ALL_SPELLS[i];
		loadedCards.push_back(new Spell(i+10, spell.cost, spell.effects));
	}
	for (int i=0; i<5; i++)
	{
		SpellData spell = ALL_SPELLS[i];
		loadedCards.push_back(new Spell(i+15, spell.cost, spell.effects));
	}
	std::shuffle(loadedCards.begin(), loadedCards.end(), _engine);
	for (int i=0; i<20; i++)
		_cardDeck.push(loadedCards.at(i));
}

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
	}
	sendHandState();
}

void Player::cardHandToBoard(int handIndex)
{
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBoard.push_back(dynamic_cast<Creature*>(_cardHand.at(handIndex)));
	_cardBoard.back()->movedToBoard();
	_cardHand.erase(handIt);
	sendHandState();
	sendBoardState();
}

void Player::cardHandToBin(int handIndex)
{
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBin.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIt);
	sendHandState();
	sendGraveyardState();
}

void Player::cardBoardToBin(int boardIndex)
{
	const auto& boardIt = std::find(_cardBoard.begin(), _cardBoard.end(), _cardBoard[boardIndex]);
	_cardBoard.at(boardIndex)->removedFromBoard();
	_cardBin.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIt);
	sendBoardState();
	sendGraveyardState();
}

void Player::cardBinToHand(int binIndex)
{
	const auto& binIt = std::find(_cardBin.begin(), _cardBin.end(), _cardBin[binIndex]);
	_cardHand.push_back(_cardBin.at(binIndex));
	_cardBin.erase(binIt);
	sendGraveyardState();
	sendHandState();
}

void Player::cardAddToHand(Card* givenCard)
{
	_cardHand.push_back(givenCard);
	sendHandState();
}

Card* Player::cardRemoveFromHand()
{
	if (_cardHand.empty())
		return nullptr;
	int handIndex = (std::uniform_int_distribution<int>(0, _cardHand.size()))(_engine);
	Card* stolenCard = _cardHand[handIndex];
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardHand.erase(handIt);
	sendHandState();
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
	sendHandState();
	return stolen;
}

void Player::sendHandState()
{
	sendIDsFromVector(TransferType::GAME_HAND_UPDATED, _cardHand);
}

void Player::sendBoardState()
{
	sendIDsFromVector(TransferType::GAME_BOARD_UPDATED, _cardBoard);
}

void Player::sendOpponentBoardState()
{
	sendIDsFromVector(TransferType::GAME_OPPONENT_BOARD_UPDATED, _opponent->getBoard());
}

void Player::sendGraveyardState()
{
	sendIDsFromVector(TransferType::GAME_GRAVEYARD_UPDATED, _cardBin);
}

// use a template to handle both Card and Creature pointers
template <typename CardType>
void Player::sendIDsFromVector(TransferType type, const std::vector<CardType *>& vect)
{
	sf::Packet packet;
	packet << type;
	std::vector<sf::Uint32> cardIds{static_cast<sf::Uint32>(vect.size())};
	for(typename std::vector<CardType *>::size_type i{0}; i < vect.size(); ++i)
		cardIds[i] = vect[i]->getID();
	packet << cardIds;
	_specialSocketToClient.send(packet);
}

void Player::sendCreatureDataFromVector(TransferType type, const std::vector<Creature*>& vect)
{
    sf::Packet packet;
	std::vector<BoardCreatureData> boardCreatures;
	BoardCreatureData data;
	for (int i=0; i<vect.size(); i++)
	{
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
		}
        boardCreatures.push_back(data);
	}
	// std::vector transmission in packet is defined in common/sockets/PacketOverload.hpp
	packet << boardCreatures;
	_specialSocketToClient.send(packet);
}

//////////

void Player::sendValueToClient(sf::TcpSocket& socket, TransferType value)
{
	sf::Packet packet;
	packet << value;
	socket.send(packet);
}

// TODO: handle the case where the user doesn't give and his turn finishes
std::vector<int>&& Player::askUserToSelectCards(const std::vector<CardToSelect>& selection)
{
	sf::Packet packet;
	packet << selection;
	_socketToClient.send(packet);
	std::vector<int> indices(selection.size());
	_socketToClient.receive(packet);
	packet >> indices;
	return std::move(indices);
}


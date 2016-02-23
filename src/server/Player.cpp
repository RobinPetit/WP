// WizardPoker headers
#include "server/Player.hpp"
#include "server/Board.hpp"
#include "server/Creature.hpp"
#include "common/sockets/TransferType.hpp"
// std-C++ headers
#include <algorithm>
// SFML headers
#include <SFML/Network/Packet.hpp>

std::function<void(Player&, const EffectParamsCollection&)> Player::effectMethods[P_EFFECTS_COUNT] =
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

/*--------------------------- BOARD INTERFACE */

void Player::enterTurn(int)
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

void Player::leaveTurn(int)
{
	//Time out player constraints
	_constraints.timeOutConstraints();

	//Time out player's creature's constraints
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->leaveTurn();

	//NETWORK: TURN_ENDED
}

void Player::useCard(int handIndex)
{
	//TODO: verify that handIndex is not out_of_range

	if (_constraints.getConstraint(PC_LIMIT_CARD_USE) == _turnData.cardsUsed)
	{
		//NETWORK: USE_CARDS_LIMIT
		return;
	}
	Card* usedCard = _cardHand.at(handIndex);

	// If card is a creature
	//TODO: use typeinfo ?
	if (usedCard->isCreature())
	{
		if (_constraints.getConstraint(PC_LIMIT_CREATURE_PLACING) == _turnData.creaturesPlaced)
		{
			//NETWORK: PLACE_CREATURES_LIMIT
			return;
		}
		_turnData.cardsUsed++;
		_turnData.creaturesPlaced++;
		cardHandToBoard(handIndex);
		exploitCardEffects(usedCard);
		//NETWORK: CREATURE_PLACED
	}

	// If card is a spell
	else
	{
		if (_constraints.getConstraint(PC_LIMIT_SPELL_CALL) == _turnData.spellCalls)
		{
			//NETWORK: CALL_SPELLS_LIMIT
			return;
		}
		_turnData.cardsUsed++;
		_turnData.spellCalls++;
		cardHandToBin(handIndex);
		exploitCardEffects(usedCard);
		//NETWORK: SPELL_CALLED
	}
}

void Player::attackWithCreature(int boardIndex, int victim)
{
	if (_constraints.getConstraint(PC_LIMIT_CREATURE_ATTACK) == _turnData.creatureAttacks)
	{
		//NETWORK: CREATURE_ATTACKS_LIMIT
		return;
	}
	int attackPoints = _cardBoard.at(boardIndex)->getAttack();
	_opponent->applyEffectToCreature(victim, CE_CHANGE_HEALTH, {attackPoints});
}

/*--------------------------- BOARD AND CREATURE INTERFACE */
void Player::applyEffectToCreature(int boardIndex, int method, const EffectParamsCollection& effectArgs)
{
	if (boardIndex<0) boardIndex = rand() % _cardBoard.size();
	Creature* usedCreature = _cardBoard.at(boardIndex);
    Creature::effectMethods[method](*usedCreature, effectArgs);
}

void Player::applyEffectToCreatures(int method, const EffectParamsCollection& effectArgs)
{
	for (unsigned i=0; i<_cardBoard.size(); i++)
	{
		applyEffectToCreature(i, method, effectArgs);
	}
}

int Player::getTeamConstraint(int constraintID)
{
	int value=0;
    for (unsigned i=0; i<_cardBoard.size(); i++)
    {
        value = _cardBoard.at(i)->getConstraint(constraintID);
        if (value != 0)
			return value;
    }
    return value;
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
	if (_energy<0) _energy=0;
}

void Player::changeEnergy(const EffectParamsCollection& args)
{
	int points = args.at(0);
	_energy+=points;
	if (_energy<0) _energy=0;
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

#include "server/Player.hpp"
#include "server/Board.hpp"
#include "server/Creature.hpp"


Player::Player(unsigned id):
	_id(id)
{
	//NETWORK: GREETINGS_USER
}

void Player::setOpponent(Player* opponent)
{
	_opponent = opponent;
}


/*--------------------------- BOARD INTERFACE */
void Player::enterTurn(unsigned)
{
	_turnData = _emptyTurnData; //Clear the turn data

	//Player's turn-based rules
	cardDeckToHand(_constraints.getConstraint(PC_CARD_PICK_AMOUNT));
	setEnergyPoints({_constraints.getConstraint(PC_ENERGY_POINTS_INIT)});
	addLifePoints({_constraints.getConstraint(PC_HEALTH_POINTS_GAIN)});
	subLifePoints({_constraints.getConstraint(PC_HEALTH_POINTS_LOSS)});
	if (_cardDeck.empty())
		subLifePoints({_constraints.getConstraint(PC_HEALTH_POINTS_LOSS_DECK_EMPTY)});

	//Will call creature's turn-based rules
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->enterTurn();

	//NETWORK: TURN_STARTED
}

void Player::leaveTurn(unsigned)
{
	//Time out player constraints
	_constraints.timeOutConstraints();

	//Time out player's creature's constraints
	for (unsigned i=0; i<_cardBoard.size(); i++)
		_cardBoard.at(i)->leaveTurn();

	//NETWORK: TURN_ENDED
}

void Player::useCard(unsigned handIndex)
{
	//TODO: verify that handIndex is not out_of_range

	if (_constraints.getConstraint(PC_USE_CARD_LIMIT) == _turnData.cardsUsed)
	{
		//NETWORK: USE_CARDS_LIMIT
		return;
	}
	Card* usedCard = _cardHand.at(handIndex);

	// If card is a creature
	//TODO: use typeinfo ?
	if (usedCard->isCreature())
	{
		if (_constraints.getConstraint(PC_PLACE_CREATURE_LIMIT) == _turnData.creaturesPlaced)
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
		if (_constraints.getConstraint(PC_CALL_SPELL_LIMIT) == _turnData.spellCalls)
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

void Player::attackWithCreature(unsigned boardIndex, unsigned victim)
{
	if (_constraints.getConstraint(PC_ATTACK_WITH_CREATURE_LIMIT) == _turnData.creatureAttacks)
	{
		//NETWORK: CREATURE_ATTACKS_LIMIT
		return;
	}
	unsigned attackPoints = _cardBoard.at(boardIndex)->getAttack();
	_opponent->applyEffectToCreature(victim, CE_SUB_HEALTH, {attackPoints});
}

/*--------------------------- BOARD AND CREATURE INTERFACE */
void Player::applyEffectToCreature(unsigned boardIndex, unsigned method, const std::vector<unsigned>& effectArgs)
{
	Creature* usedCreature = _cardBoard.at(boardIndex);
	(usedCreature->*(usedCreature->effectMethods[method]))(effectArgs);
}

void Player::applyEffectToCreatures(unsigned method, const std::vector<unsigned>& effectArgs)
{
	for (unsigned i=0; i<_cardBoard.size(); i++)
	{
		applyEffectToCreature(i, method, effectArgs);
	}
}

std::pair<unsigned, unsigned> Player::getTeamConstraint(unsigned constraintID)
{
    for (unsigned i=0; i<_cardBoard.size(); i++)
    {
        unsigned value = _cardBoard.at(i)->getConstraint(constraintID);
        if (value != 0)
		return std::make_pair(value, i);
    }
    return std::make_pair(0U,0U);
}

/*--------------------------- EFFECTS */
void Player::setConstraint(const std::vector<unsigned>& args)
{
	unsigned constraintID = args.at(0);
	unsigned value = args.at(1);
	unsigned turns = args.at(2);
	_constraints.setConstraint(constraintID, value, turns);
}

void Player::pickDeckCards(const std::vector<unsigned>& args)
{
	cardDeckToHand(args.at(0));
}

void Player::loseHandCards(const std::vector<unsigned>& args)
{
	unsigned amount = args.at(1);
	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		unsigned handIndex = rand() % _cardHand.size();
		cardHandToBin(handIndex);
	}
}

void Player::reviveBinCard(const std::vector<unsigned>& args)
{
    unsigned binIndex = args.at(0);
	cardBinToHand(binIndex);
}

void Player::stealHandCard(const std::vector<unsigned>&)
{
    cardAddToHand(_opponent->cardRemoveFromHand());
}

void Player::exchgHandCard(const std::vector<unsigned>& args)
{
	//TODO: check index
	unsigned myCardIndex = args.at(0);
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

void Player::setEnergyPoints(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	_energyPoints = points;
}

void Player::addEnergyPoints(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	_energyPoints += points;
}

void Player::subEnergyPoints(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	if (_energyPoints > points)
		_energyPoints -= points;
	else
	{
		_energyPoints = 0;
		//NETWORK NO_MORE_ENERGY
		//call endTurn()
	}
}

void Player::addLifePoints(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	_healthPoints += points;
}

void Player::subLifePoints(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	if (_healthPoints > points)
		_healthPoints -= points;
	else
	{
		_healthPoints = 0;
		//call die()
	}
}


/*--------------------------- PRIVATE */
void Player::exploitCardEffects(Card* usedCard)
{
	std::vector<std::vector<unsigned>> effects = usedCard->getEffects();
	for (unsigned i=0; i<effects.size(); i++) //for each effect of the card
	{
		_board->applyEffect(usedCard, effects.at(i)); //apply it
	}
}

void Player::cardDeckToHand(unsigned amount)
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

void Player::cardHandToBoard(unsigned handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBoard.push_back(dynamic_cast<Creature*>(_cardHand.at(handIndex)));
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: BOARD_CHANGED
}

void Player::cardHandToBin(unsigned handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBin.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: BIN_CHANGED
}

void Player::cardBoardToBin(unsigned boardIndex)
{
	const auto& boardIt = std::find(_cardBoard.begin(), _cardBoard.end(), _cardBoard[boardIndex]);
	_cardBin.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIt);
	//NETWORK: BOARD_CHANGED
	//NETWORK: BIN_CHANGED
}

void Player::cardBinToHand(unsigned binIndex)
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
	if (_cardHand.empty()) return nullptr;

	unsigned handIndex = rand() % _cardHand.size();
	Card* stolenCard = _cardHand[handIndex];
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: CARD_STOLEN
	return stolenCard;
}

Card* Player::cardExchangeFromHand(Card* givenCard)
{
	unsigned handIndex = rand() % _cardHand.size();
	return cardExchangeFromHand(givenCard, handIndex);
}

Card* Player::cardExchangeFromHand(Card* givenCard, unsigned handIndex)
{
	if (_cardHand.empty()) return nullptr;

	Card* stolen = _cardHand[handIndex];
	_cardHand.at(handIndex) = givenCard;
	//NETWORK: HAND_CHANGED
	//NETWORK: CARD_EXCHANGED
	return stolen;
}

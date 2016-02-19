#include "server/Player.hpp"
#include "server/Board.hpp"


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
void Player::enterTurn(unsigned turn)
{
	//Player's turn-based rules
	_turnData = _emptyTurnData; //Clear the turn data
	cardPickFromDeck(_constraints.getConstraint(PC_CARD_PICK_AMOUNT));
	setEnergyPoints({_constraints.getConstraint(PC_ENERGY_POINTS_INIT)});
	addLifePoints({_constraints.getConstraint(PC_HEALTH_POINTS_GAIN)});
	subLifePoints({_constraints.getConstraint(PC_HEALTH_POINTS_LOSS)});
	if (_cardDeck.empty())
		subLifePoints({_constraints.getConstraint(PC_HEALTH_POINTS_LOSS_DECK_EMPTY)});
	//Call creature's turn-based rules
	//NETWORK: TURN_STARTED
}

void Player::leaveTurn(unsigned turn)
{
	_constraints.timeOutConstraints();
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
		cardPlaceOnBoard(handIndex);
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
		cardDiscardFromHand(handIndex);
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
	//attack with _cardBoard.at(boardIndex) against victim
}

/*--------------------------- EFFECTS */
void Player::setConstraint(std::vector<unsigned> args)
{
	unsigned constraintID = args.at(0);
	unsigned value = args.at(1);
	unsigned turns = args.at(2);
	_constraints.setConstraint(constraintID, value, turns);
}

void Player::pickDeckCards(std::vector<unsigned> args)
{
	cardPickFromDeck(args.at(0));
}

void Player::loseHandCards(std::vector<unsigned> args)
{
	unsigned amount = args.at(1);
	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		unsigned handIndex = rand() % _cardHand.size();
		cardDiscardFromHand(handIndex);
	}
}

void Player::stealHandCard(std::vector<unsigned> args)
{
	_cardHand.push_back(_opponent->cardRemoveFromHand());
}

void Player::exchgHandCard(std::vector<unsigned> args)
{
	unsigned myCardIndex = args.at(0);
	Card* myCard = _cardHand.at(myCardIndex);

	_cardHand.at(myCardIndex) = _opponent->cardExchangeFromHand(myCard);
	if (_cardHand.at(myCardIndex) == nullptr)
	{
		const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[myCardIndex]);
		_cardHand.erase(handIt);
	}
}

void Player::setEnergyPoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_energyPoints = points;
}

void Player::addEnergyPoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_energyPoints += points;
}

void Player::subEnergyPoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	if (_energyPoints > points) _energyPoints -= points;
	else
	{
		_energyPoints = 0;
		//NETWORK NO_MORE_ENERGY
		//call endTurn()
	}
}

void Player::addLifePoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_healthPoints += points;
}

void Player::subLifePoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	if (_healthPoints > points) _healthPoints -= points;
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

void Player::cardPickFromDeck(unsigned amount)
{
	if(not _cardDeck.empty() and amount>0)
	{
		amount--;
		_cardHand.push_back(_cardDeck.top());
		_cardDeck.pop();
		//NETWORK: DECK_EMPTY
	}
	//NETWORK: BOARD_CHANGED
	//NETWORK: DECK_CHANGED
}

void Player::cardPlaceOnBoard(unsigned handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBoard.push_back(dynamic_cast<Creature*>(_cardHand.at(handIndex)));
	_cardHand.erase(handIt);
	//NETWORK: BOARD_CHANGED
	//NETWORK: DECK_CHANGED
}

void Player::cardDiscardFromHand(unsigned handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBin.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIt);
	//NETWORK: BIN_CHANGED
	//NETWORK: HAND_CHANGED
}

void Player::cardDiscardFromBoard(unsigned boardIndex)
{
	const auto& boardIt = std::find(_cardBoard.begin(), _cardBoard.end(), _cardBoard[boardIndex]);
	_cardBin.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIt);
	//NETWORK: BOARD_CHANGED
	//NETWORK: BIN_CHANGED
}

Card* Player::cardRemoveFromHand()
{
	if (_cardHand.empty()) return nullptr;

	unsigned handIndex = rand() % _cardHand.size();
	Card* stolen = _cardHand[handIndex];
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardHand.erase(handIt);
	//NETWORK: HAND_CHANGED
	//NETWORK: CARD_STOLEN
	return stolen;
}

Card* Player::cardExchangeFromHand(Card* given)
{
	if (_cardHand.empty()) return nullptr;

	unsigned handIndex = rand() % _cardHand.size();
	Card* stolen = _cardHand[handIndex];
	_cardHand.at(handIndex) = given;
	//NETWORK: HAND_CHANGED
	//NETWORK: CARD_EXCHANGED
	return stolen;
}

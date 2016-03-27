// WizardPoker headers
#include "client/Gui/GuiGame.hpp"
#include "client/sockets/Client.hpp"
// External headers
#include <TGUI/Widgets/MessageBox.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Animation.hpp>

constexpr int GuiGame::NoSelection;

//////////////////// GuiGame

GuiGame::GuiGame(Context& context):
	AbstractGame{*context.client},
	_context{context},
	_decksListBox{std::make_shared<tgui::ListBox>()},
	_decksChosen{false},
	_display{false},
	_width{tgui::bindWidth(*_context.gui)},
	_height{tgui::bindHeight(*_context.gui)},
	_selfHandPanel{std::make_shared<tgui::Panel>()},
	// Self info labels
	_selfInfoLayout{std::make_shared<tgui::VerticalLayout>()},
	_selfHealthLabel{std::make_shared<tgui::Label>()},
	_selfEnergyLabel{std::make_shared<tgui::Label>()},
	_selfDeckSizeLabel{std::make_shared<tgui::Label>()},
	_selfGraveyardSizeLabel{std::make_shared<tgui::Label>()},
	_endTurnButton{std::make_shared<tgui::Button>()},
	_attackOpponentButton{std::make_shared<tgui::Button>()},
	// Opponent info layout
	_oppoInfoLayout{std::make_shared<tgui::VerticalLayout>()},
	_oppoHealthLabel{std::make_shared<tgui::Label>()},
	_oppoHandSizeLabel{std::make_shared<tgui::Label>()},
	_cardsLayoutWidth{_width/1.4f},
	_isBigCardOnBoard{false},
	_opponentBoardPanel{std::make_shared<tgui::Panel>()},
	_selfBoardPanel{std::make_shared<tgui::Panel>()},
	_currentSelfSelection{NoSelection},
	_currentOpponentSelection{NoSelection},
	_activeCallbacks{true},
	_displayRequest{false},
	_ownerId{std::this_thread::get_id()}
{
	// end turn button
	_endTurnButton->setText("End turn");
	_endTurnButton->connect("Pressed", [this]()
	{
		endTurn();
		_endTurnButton->disable();
		displayGame();
	});

	float heightWidget{_height.getValue()};

	// have the grid sized so that we can align perfectly 4 cards in it
	auto cardWidth{_cardsLayoutWidth / 4.f};
	auto cardHeight{cardWidth * (CardGui::getSize().y / static_cast<float>(CardGui::getSize().x))};
	_selfHandPanel->setSize(_cardsLayoutWidth, cardHeight);
	// remove the self hand panel
	heightWidget -= _selfHandPanel->getSize().y + 10;
	_selfHandPanel->setPosition({(_width - _cardsLayoutWidth) / 2.f, heightWidget});
	_context.gui->add(_selfHandPanel);

	// Self info labels
	_selfHealthLabel->setTextColor(sf::Color(200, 0, 0));  // Dark red
	_selfHealthLabel->setTextSize(15);
	_selfEnergyLabel->setTextColor(sf::Color(0, 0, 200));  // Dark blue
	_selfEnergyLabel->setTextSize(15);
	_selfDeckSizeLabel->setTextSize(15);
	_selfGraveyardSizeLabel->setTextSize(15);
	// The text of these labels is set in displayGame
	_selfInfoLayout->add(_selfHealthLabel);
	_selfInfoLayout->add(_selfEnergyLabel);
	_selfInfoLayout->add(_selfDeckSizeLabel);
	_selfInfoLayout->add(_selfGraveyardSizeLabel);
	_selfInfoLayout->add(_endTurnButton);
	_selfInfoLayout->setSize((_width - _cardsLayoutWidth) / 2.f - 10.f, tgui::bindHeight(_selfHandPanel));
	_selfInfoLayout->setPosition(tgui::bindRight(_selfHandPanel) + 5.f, tgui::bindTop(_selfHandPanel));
	_context.gui->add(_selfInfoLayout);

	// default behaviour of button is to be disabled (re-enabled at the beginning of each turn)
	_endTurnButton->disable();

	// Oponnent info labels
	_oppoHealthLabel->setTextColor(sf::Color(200, 0, 0));  // Dark red
	_oppoHealthLabel->setTextSize(15);
	_oppoHandSizeLabel->setTextSize(15);
	// The text of these labels is set in displayGame
	_oppoInfoLayout->add(_oppoHealthLabel);
	_oppoInfoLayout->add(_oppoHandSizeLabel);
	_oppoInfoLayout->setSize((_width - _cardsLayoutWidth) / 2.f - 10.f, tgui::bindHeight(_selfHandPanel));
	_oppoInfoLayout->setPosition(5.f, tgui::bindTop(_selfHandPanel));
	_context.gui->add(_oppoInfoLayout);

	// self board
	_selfBoardPanel->setBackgroundColor(sf::Color(50, 50, 50));
	_selfBoardPanel->setSize(_width, 180);
	// remove the self board panel
	heightWidget -= _selfBoardPanel->getSize().y + 20;
	_selfBoardPanel->setPosition({0.f, heightWidget});
	_context.gui->add(_selfBoardPanel);

	// opponent board
	_opponentBoardPanel->setBackgroundColor(sf::Color(100, 100, 100));
	_opponentBoardPanel->setSize(_width, 180);
	// remove the opponent board panel
	heightWidget -= _opponentBoardPanel->getSize().y + 20;
	_opponentBoardPanel->setPosition({0.f, heightWidget});
	_context.gui->add(_opponentBoardPanel);

	// Place the button to attack directly the opponent
	_attackOpponentButton->setText("Attack opponent");
	_attackOpponentButton->setSize(_width * 3.f / 4.f, _opponentBoardPanel->getPosition().y - 10);
	_attackOpponentButton->setPosition(_width / 8.f, 5);
	_attackOpponentButton->connect("Pressed", [this]()
	{
		int selfSelection{_currentSelfSelection};
		unselectSelfCard();
		attackWithCreature(selfSelection, true, NoSelection);
		displayGame();
	});
	_context.gui->add(_attackOpponentButton);
}

void GuiGame::startTurn()
{
	AbstractGame::startTurn();
	_endTurnButton->enable();

	_display = false;
	handleInputs();
}

void GuiGame::handleInputs()
{
	// waitUntil handles the events itself
	waitUntil([this]()
	{
		return not _myTurn.load();
	});
}

void GuiGame::clearScreen()
{
	_context.window->clear(sf::Color::White);
}

void GuiGame::refreshScreen()
{
	assert(std::this_thread::get_id() == _ownerId);
	clearScreen();
	_context.gui->draw();
	_context.window->display();
}

void GuiGame::processWindowEvents()
{
	sf::Event event;
	while(_context.window->pollEvent(event))
	{
		assert(std::this_thread::get_id() == _ownerId);
		if(_displayRequest.load())
		{
			displayGame();
			_displayRequest.store(false);
		}
		if(event.type == sf::Event::Closed)
			quit();
		else
			_context.gui->handleEvent(event);
	}
}

void GuiGame::displayGame()
{
	// don't forget to lock the screen!
	std::lock_guard<std::mutex> _lock{_accessScreen};
	assert(std::this_thread::get_id() == _ownerId);

	displayHandCards();
	displaySelfBoard();
	displayOpponentBoard();
	displayInfo();

	updateGuiCardValues();

	refreshScreen();
}

void GuiGame::updateDisplay()
{
	if(_playing.load())
		_displayRequest.store(true);
}

void GuiGame::onListeningThreadCreation()
{
	assert(_context.window->setActive(false));
	sf::Context context;
}

void GuiGame::displayHandCards()
{
	static auto availableWidth{_cardsLayoutWidth * 3.f / 4.f};
	auto cardHeight{_selfHandPanel->getSize().y};

	// Clear the panel
	_selfHandPanel->removeAllWidgets();
	_selfHand.clear();

	// do not use an iterator-for loop since i is a used counter variable
	const float distanceBetweenCards{(availableWidth/(static_cast<float>(_selfHandCards.size()) - 1.f)).getValue()};
	for(unsigned i{0U}; i < _selfHandCards.size(); ++i)
	{
		const auto cardData{_context.client->getCardData(_selfHandCards.at(i).id)};
		const auto widthPosition{static_cast<float>(i) * distanceBetweenCards};
		// create a new card
		_selfHand.push_back(std::make_shared<CardWidget>(cardData));
		// Have it sized according to the panel
		_selfHand.back()->setSize(_cardsLayoutWidth / 4.f, cardHeight);
		// Place it one the panel
		_selfHand.back()->setPosition({widthPosition, 0.f});

		////////// Set up callbacks

		// If card is pressed, use the card
		_selfHand.back()->connect("MousePressed", [this, i]()
		{
			useCard(i);
			displayGame();
		});
		// When the mouse enters on the card area
		connectBigCardDisplay(_selfHand.back(), cardData);
		// Add the card to the panel
		_selfHandPanel->add(_selfHand.back());
	}
}

void GuiGame::displayInfo()
{
	_selfHealthLabel->setText("Health: " + std::to_string(_selfHealth));
	_selfEnergyLabel->setText("Energy: " + std::to_string(_selfEnergy));
	_selfDeckSizeLabel->setText("Deck size: " + std::to_string(_selfDeckSize));
	_selfGraveyardSizeLabel->setText("Grave size: " + std::to_string(_selfGraveCards.size()));

	_oppoHealthLabel->setText("Opponent's health: " + std::to_string(_oppoHealth));
	_oppoHandSizeLabel->setText("Opponent's hand size: " + std::to_string(_oppoHandSize));
}

void GuiGame::connectBigCardDisplay(CardWidget::Ptr& card, const CommonCardData *cardData, const BoardCreatureData *data)
{
	if(not data)
		card->connect("MouseEntered", &GuiGame::displayBigCard, this, cardData);
	else
		card->connect("MouseEntered", &GuiGame::displayBigCreature, this, cardData, *data);
	// When the mouse leaves the card area
	card->connect("MouseLeft", [this]()
	{
		if(not _readableCard)
			return;
		_context.gui->remove(_readableCard);
		_isBigCardOnBoard = false;
		refreshScreen();
	});
}

void GuiGame::displaySelfBoard()
{
	displayPlayerBoard(_selfBoardPanel, _selfBoard, _selfBoardCreatures,
		_activeCallbacks ? &GuiGame::handleSelfBoardClick : nullptr);
}

void GuiGame::displayOpponentBoard()
{
	displayPlayerBoard(_opponentBoardPanel, _opponentBoard,  _oppoBoardCreatures,
		_activeCallbacks ? &GuiGame::handleOpponentBoardClick : nullptr, true);
}

void GuiGame::handleSelfBoardClick(int index)
{
	if(not _myTurn.load())
	{
		displayMessage("You must wait your turn!");
		return;
	}
	if(_currentSelfSelection == index)
		unselectSelfCard();
	else
	{
		_currentSelfSelection = index;
		_selfBoard.at(index)->select();
		for(auto& card : _opponentBoard)
			card->aimable();
		_attackOpponentButton->show();
	}
}

void GuiGame::handleOpponentBoardClick(int index)
{
	if(not _myTurn.load())
	{
		displayMessage("You must wait your turn!");
		return;
	}
	// clicking on opponent if selection is null doesn't do anything
	if(_currentSelfSelection == NoSelection)
		return;
	_currentOpponentSelection = index;
	int selfSelection{_currentSelfSelection};
	unselectSelfCard();
	attackWithCreature(selfSelection, false, _currentOpponentSelection);
	displayGame();
}

void GuiGame::removeClickableCallbacks()
{
	_activeCallbacks = false;
	displayGame();
}

void GuiGame::unselectSelfCard()
{
	if(_currentSelfSelection == NoSelection)
		return;
	_selfBoard.at(_currentSelfSelection)->unselect();
	_currentSelfSelection = NoSelection;
	for(auto& card : _opponentBoard)
		card->unaimable();
	_attackOpponentButton->hide();
}

void GuiGame::displayPlayerBoard(tgui::Panel::Ptr& panel, DisplayableCardsCollection& graphicalCards,
		std::vector<BoardCreatureData>& creatureDatas, CardCallback callback,
		bool reversed, bool displayableWhenMouseOver)
{
	const auto cardHeight{panel->getSize().y};
	const auto cardWidth{cardHeight * (CardGui::getSize().x / static_cast<float>(CardGui::getSize().y))};
	const unsigned int nbOfPossibleCards{static_cast<unsigned int>(panel->getSize().x / cardWidth)};
	const float widthBetweenCards{(panel->getSize().x / static_cast<float>(nbOfPossibleCards)) - cardWidth};

	panel->removeAllWidgets();
	graphicalCards.clear();

	const auto nbOfCards{creatureDatas.size()};
	// the width that's needed to display the cards is n * cardWidth + (n-1) * space between cards
	// where n is the number of cards that are displayed
	const float usedWidth{static_cast<float>(nbOfCards) * (cardWidth + widthBetweenCards) - widthBetweenCards};
	// start at the first place to put a card
	float currentWidth{(panel->getSize().x - usedWidth) / 2.f};

	for(auto i{0U}; i < nbOfCards; ++i)
	{
		CardId monsterId{creatureDatas.at(i).id};
		const auto cardData{_context.client->getCardData(monsterId)};
		// create the new card to display
		graphicalCards.push_back(std::make_shared<CardWidget>(cardData));
		// have it resized to fit in the panel
		graphicalCards.back()->setSize(cardWidth, cardHeight);
		// place it correctly (and rotate if needed)
		if(reversed)
		{
			// set the origin on the lower right corner
			graphicalCards.back()->setOrigin(CardGui::getSize().x, CardGui::getSize().y);
			graphicalCards.back()->rotate(180.f);
			graphicalCards.back()->setPosition(currentWidth + CardGui::getSize().x/2.f - cardWidth,
					(CardGui::getSize().y/2.f - cardHeight));
		}
		else
			graphicalCards.back()->setPosition(currentWidth, 0.f);

		if(displayableWhenMouseOver)
			connectBigCardDisplay(graphicalCards.back(), cardData, &creatureDatas.at(i));

		if(callback)
			graphicalCards.back()->connect("MousePressed", callback, this, i);
		// update the current position on the panel
		currentWidth += cardWidth;

		panel->add(graphicalCards.back());
		// move a bit away to separate cards
		currentWidth += widthBetweenCards;
	}
}

void GuiGame::displayBigCard(const CommonCardData *cardData)
{
	if(_isBigCardOnBoard)
		return;
	createBigCard(cardData);
	_isBigCardOnBoard = true;
	_context.gui->add(_readableCard);
	refreshScreen();
}

void GuiGame::displayBigCreature(const CommonCardData *cardData, const BoardCreatureData& data)
{
	if(_isBigCardOnBoard)
		return;
	createBigCard(cardData);
	_isBigCardOnBoard = true;
	updateCard(dynamic_cast<CreatureGui *>(_readableCard->getCard()), data);
	_context.gui->add(_readableCard);
	refreshScreen();
}

void GuiGame::createBigCard(const CommonCardData *cardData)
{
	_context.gui->remove(_readableCard);
	// make a new card, center it and then display it
	_readableCard.reset(new CardWidget(cardData));
	_readableCard->setPosition((tgui::bindWidth (*_context.gui) - tgui::bindWidth (_readableCard)) / 2,
				   (tgui::bindHeight(*_context.gui) - tgui::bindHeight(_readableCard)) / 2);
}

// NOTE: this is copy/paste from GuiAbstractState. TODO: factorize by creating an interface
// Messageable, GuiMessageable, TerminalMessageable to factorize this code.
void GuiGame::displayMessage(const std::string& message)
{
	std::lock_guard<std::mutex> _lock{_accessScreen};
	static const std::string okButtonText{"Ok"};
	tgui::MessageBox::Ptr messageBox{std::make_shared<tgui::MessageBox>()};

	// Set up the message box
	messageBox->setText(message);
	messageBox->addButton(okButtonText);
	messageBox->getRenderer()->setTitleBarColor({127, 127, 127});
	_context.gui->add(messageBox);
	messageBox->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(messageBox)) / 2,
			(tgui::bindHeight(*_context.gui) - tgui::bindHeight(messageBox)) / 2);

	// Make the "Ok" button closing the message box
	// Note: do not try to pass messageBox as reference, since this lambda will
	// be stored elsewhere, the reference will become invalid when we'll go out
	// of the scope of this method! We must pass it by value.
	messageBox->connect("buttonPressed", [this, messageBox](const sf::String& buttonName)
	{
		if(buttonName == okButtonText)
		{
			messageBox->destroy();
			refreshScreen();
		}
	});
	_context.gui->draw();
}

int GuiGame::askIndexFromVector(DisplayableCardsCollection& cards)
{
	int chosenIndex;
	bool isIndexChosen{false};
	removeClickableCallbacks();
	unsigned int i{0U};

	for(auto& card : cards)
	{
		card->selectable();
		card->connect("MousePressed", [this, &chosenIndex, &isIndexChosen, i]()
		{
			chosenIndex = i;
			isIndexChosen = true;
		});
		++i;
	}
	waitUntil([&isIndexChosen]()
	{
		return isIndexChosen;
	});
	return chosenIndex;
}

int GuiGame::askSelfHandIndex()
{
	return askIndexFromVector(_selfHand);
}

int GuiGame::askSelfBoardIndex()
{
	return askIndexFromVector(_selfBoard);
}

int GuiGame::askOppoBoardIndex()
{
	return askIndexFromVector(_opponentBoard);
}

// <TODO>
int GuiGame::askSelfGraveyardIndex()
{
	return -42;
}

int GuiGame::askOppoHandIndex()
{
	return -42;
}
// </TODO>

void GuiGame::chooseDeck()
{
	// Hide the panels and the end turn button
	_selfBoardPanel->hide();
	_opponentBoardPanel->hide();
	_selfHandPanel->hide();
	_endTurnButton->hide();
	_attackOpponentButton->hide();
	// get all of the decks
	std::vector<Deck> decks{_client.getDecks()};

	// list them all in a listbox
	for(const auto& deck : decks)
		_decksListBox->addItem(deck.getName());

	_decksListBox->setPosition(_width/4.f, _height/4.f);
	_decksListBox->setSize(_width/2.f, _height/4.f);
	_context.gui->add(_decksListBox);

	tgui::Button::Ptr selectButton{std::make_shared<tgui::Button>()};

	selectButton->setText("Select");
	selectButton->setPosition(_width/4.f, _height*3.f/4.f);
	selectButton->setSize(_width/2.f, _height/5.f);
	selectButton->connect("pressed", [this]()
	{
		// if a deck is selected, then use this one, otherwise keep waiting
		const std::string& selection{_decksListBox->getSelectedItem()};
		if(selection == "")
			displayMessage("You need to choose a deck\n");
		else
			sendDeck(_decksListBox->getSelectedItem());
	});
	_context.gui->add(selectButton);

	// Have an inner event loop to not leave the function as long as deck has not been selected
	waitUntil([this]()
	{
		return _decksChosen;
	});
	// clean the GUI
	_context.gui->remove(selectButton);
	_context.gui->remove(_decksListBox);

	// show back in-game widgets
	_selfBoardPanel->show();
	_opponentBoardPanel->show();
	_selfHandPanel->show();
	_endTurnButton->show();
}

void GuiGame::sendDeck(const std::string& deckName)
{
	_decksChosen = true;
	AbstractGame::sendDeck(deckName);
}

void GuiGame::receiveCard(CardId id)
{
	// TODO: show the card
	displayMessage("You won the card " + getCardName(id) + ".");
}

void GuiGame::displayAchievements(ClientAchievementList& newAchievements)
{
	//No new achievements
	if (newAchievements.size()==0)
		return;

	std::lock_guard<std::mutex> _lock{_accessScreen};
	static const std::string okButtonText{"Ok"};
	tgui::MessageBox::Ptr messageBox{std::make_shared<tgui::MessageBox>()};

	//set up message box
	messageBox->setText("New achievements unlocked !");
	messageBox->addButton(okButtonText);
	messageBox->getRenderer()->setTitleBarColor({127, 127, 127});
	_context.gui->add(messageBox);
	messageBox->setPosition((tgui::bindWidth(*_context.gui) - AchievementGui::getSize().x) / 2,
			(tgui::bindHeight(*_context.gui) - AchievementGui::getSize().y) / 2);
	messageBox->setSize(AchievementGui::getSize().x, AchievementGui::getSize().y);

	// Make the "Ok" button closing the message box
	// Note: do not try to pass messageBox as reference, since this lambda will
	// be stored elsewhere, the reference will become invalid when we'll go out
	// of the scope of this method! We must pass it by value.
	messageBox->connect("buttonPressed", [this, messageBox](const sf::String& buttonName)
	{
		if(buttonName == okButtonText)
		{
			messageBox->destroy();
			refreshScreen();
		}
	});

	// set up vertical layout
	tgui::VerticalLayout::Ptr layout{std::make_shared<tgui::VerticalLayout>()};
	layout->setPosition(0.f, 0.f);
	layout->setSize(AchievementGui::getSize().x, AchievementGui::getSize().y);
	messageBox->add(layout);

	_context.gui->draw();

	// create the achievement widgets
	std::vector<std::shared_ptr<AchievementWidget>> achievementWidgets{};
	unsigned int i{0};
	for(auto& achievement : newAchievements)
	{
		achievementWidgets.push_back(std::make_shared<AchievementWidget>(achievement));
		layout->add(achievementWidgets.back());
		++i;
	}
	layout->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, sf::milliseconds(500));
}

void GuiGame::waitUntil(std::function<bool()> booleanFunction)
{
	while(not booleanFunction())
	{
		refreshScreen();
		processWindowEvents();
	}
}

void GuiGame::updateGuiCardValues()
{
	updateGuiBoard(_selfBoardCreatures, _selfBoard);
	updateGuiBoard(_oppoBoardCreatures, _opponentBoard);
}

void GuiGame::updateGuiBoard(std::vector<BoardCreatureData>& dataBoard, DisplayableCardsCollection& guiBoard)
{
	assert(dataBoard.size() == guiBoard.size());
	for(auto i{0U}; i < dataBoard.size(); ++i)
	{
		auto& guiCreature{guiBoard.at(i)};
		auto& creatureDatas{dataBoard.at(i)};
		CreatureGui *currentCard{dynamic_cast<CreatureGui *>(guiCreature->getCard())};
		assert(currentCard != nullptr);

		updateCard(currentCard, creatureDatas);
	}
}

void GuiGame::updateCard(CreatureGui *card, const BoardCreatureData& data)
{
	card->setHealth(data.health);
	card->setAttack(data.attack);
	card->setShield(data.shield);
}

GuiGame::~GuiGame()
{
	_context.gui->remove(_selfHandPanel);
	_context.gui->remove(_selfBoardPanel);
	_context.gui->remove(_opponentBoardPanel);
	if(_isBigCardOnBoard)
		_context.gui->remove(_readableCard);
	_context.gui->remove(_oppoInfoLayout);
	_context.gui->remove(_attackOpponentButton);
	_context.gui->remove(_endTurnButton);
	_context.gui->remove(_selfInfoLayout);
}

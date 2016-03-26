// WizardPoker headers
#include "client/Gui/GuiGame.hpp"
#include "client/sockets/Client.hpp"
// External headers
#include <TGUI/Widgets/MessageBox.hpp>
#include <TGUI/Widgets/Button.hpp>

//////////////////// GuiGame

GuiGame::GuiGame(Context& context):
	AbstractGame{*context.client},
	_context{context},
	_decksListBox{std::make_shared<tgui::ListBox>()},
	_decksChosen{false},
	_width{tgui::bindWidth(*_context.gui)},
	_height{tgui::bindHeight(*_context.gui)},
	_selfHandPanel{std::make_shared<tgui::Panel>()},
	_endTurnButton{std::make_shared<tgui::Button>()},
	_cardsLayoutWidth{_width/1.4f},
	_isBigCardOnBoard{false}
{
	// have the grid sized so that we can align perfectly 4 cards in it
	_selfHandPanel->setSize(_cardsLayoutWidth, (_cardsLayoutWidth/4.f * (CardGui::heightCard / static_cast<float>(CardGui::widthCard))));
	_selfHandPanel->setPosition((_width - _cardsLayoutWidth) / 2.f, _height - (_selfHandPanel->getSize().y+10));

	// end turn button
	_endTurnButton->setText("End turn");
	_endTurnButton->connect("Pressed", [this]()
	{
		endTurn();
		_endTurnButton->disable();
	});
	// place it right to the cards
	_endTurnButton->setSize((_width - _cardsLayoutWidth) / 2 - 10, 40);
	_endTurnButton->setPosition((_width + _cardsLayoutWidth) / 2 + 5, _height - 50);
	// default behaviour of button is to be disabled (re-enabled at the beginning of each turn)
	_endTurnButton->disable();
}

void GuiGame::startTurn()
{
	AbstractGame::startTurn();
	_endTurnButton->enable();

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

void GuiGame::refreshScreen()
{
	_context.window->clear(sf::Color::White);
	_context.gui->draw();
	_context.window->display();
}

void GuiGame::displayGame()
{
	// don't forget to lock the screen!
	std::lock_guard<std::mutex> _lock{_accessScreen};
	//std::vector<CardWidget::Ptr> selfBoard;
	//std::vector<CardWidget::Ptr> opponentBoard;

	// start by removing everything from the current window
	_context.gui->remove(_endTurnButton);

	displayHandCards();

	_endTurnButton->setSize((_width - _cardsLayoutWidth) / 2 - 10, 40);
	_context.gui->add(_endTurnButton);

	if(_isBigCardOnBoard)
		_context.gui->add(_readableCard);

	refreshScreen();
}

void GuiGame::displayHandCards()
{
	static auto availableWidth{_cardsLayoutWidth * 3.f / 4.f};
	auto cardHeight{_selfHandPanel->getSize().y};
	std::vector<CardWidget::Ptr> selfHand;

	// Clear the panel
	_selfHandPanel->removeAllWidgets();

	// do not use an iterator-for loop since i is a used counter variable
	const float distanceBetweenCards{(availableWidth/(static_cast<float>(_selfHandCards.size()) - 1.f)).getValue()};
	std::cout << "Distance beteen cards: " << distanceBetweenCards << std::endl;
	for(unsigned i{0U}; i < _selfHandCards.size(); ++i)
	{
		const auto cardData{_context.client->getCardData(_selfHandCards.at(i).id)};
		const auto widthPosition{static_cast<float>(i) * distanceBetweenCards};
		// create a new card
		selfHand.push_back(std::make_shared<CardWidget>(cardData));
		// Have it sized according to the panel
		selfHand.back()->setSize(_cardsLayoutWidth / 4.f, cardHeight);
		// Place it one the panel
		selfHand.back()->setPosition({widthPosition, 0.f});

		////////// Set up callbacks

		// If card is pressed, use the card
		selfHand.back()->connect("MousePressed", [this, i]()
		{
			useCard(i);
		});
		// When the mouse enters on the card area
		// cardData can be captured by value since it isa pointer
		selfHand.back()->connect("MouseEntered", [this, cardData]()
		{
			if(_isBigCardOnBoard)
				return;
			// make a new card, center it and then display it
			_readableCard.reset(new CardWidget(cardData));
			_readableCard->setPosition((tgui::bindWidth (*_context.gui) - tgui::bindWidth (_readableCard)) / 2,
			                           (tgui::bindHeight(*_context.gui) - tgui::bindHeight(_readableCard)) / 2);
			_isBigCardOnBoard = true;
		});
		// When the mouse leaves the card area
		selfHand.back()->connect("MouseLeft", [this]()
		{
			_readableCard->hide();
			_isBigCardOnBoard = false;
		});
		// Add the card to the panel
		_selfHandPanel->add(selfHand.back());
	}
	// add the panel on the Gui to be displayed
	_context.gui->add(_selfHandPanel);
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
	messageBox->connect("buttonPressed", [messageBox](const sf::String& buttonName)
	{
		if(buttonName == okButtonText)
			messageBox->destroy();
	});
	_context.gui->draw();
}

// <TODO>
int GuiGame::askSelfHandIndex()
{
	return -42;
}

int GuiGame::askSelfBoardIndex()
{
	return -42;
}

int GuiGame::askSelfGraveyardIndex()
{
	return -42;
}

int GuiGame::askOppoBoardIndex()
{
	return -42;
}

int GuiGame::askOppoHandIndex()
{
	return -42;
}

bool GuiGame::wantToAttackOpponent()
{
	return false;
}
// </TODO>

void GuiGame::chooseDeck()
{
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
	sf::Event event;
	while(not _decksChosen)
	{
		_context.window->clear(sf::Color::White);
		_context.gui->draw();
		_context.window->display();

		// Use waitEvent to block everything
		_context.window->waitEvent(event);
		_context.gui->handleEvent(event);
	}
	// clean the GUI
	_context.gui->removeAllWidgets();
}

void GuiGame::sendDeck(const std::string& deckName)
{
	_decksChosen = true;
	AbstractGame::sendDeck(deckName);
}

void GuiGame::receiveCard(cardId id)
{
	// TODO: show the card
	displayMessage("You won the card " + getCardName(id) + ".");
}

void GuiGame::displayAchievements(ClientAchievementList& /* newAchievements */)
{
	// TODO
}

void GuiGame::waitUntil(std::function<bool()> booleanFunction)
{
	sf::Event event;
	while(not booleanFunction())
	{
		displayGame();
		while(not _context.window->pollEvent(event))
			sf::sleep(sf::milliseconds(10));
		do
		{
			if(event.type == sf::Event::Closed)
				quit();
			else
				_context.gui->handleEvent(event);
		}
		while(_context.window->pollEvent(event));
	}
}

GuiGame::~GuiGame()
{
	_context.gui->removeAllWidgets();
}

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
	_selfHandLayout{std::make_shared<tgui::HorizontalLayout>()},
	_endTurnButton{std::make_shared<tgui::Button>()},
	_cardsLayoutWidth{_width/1.4f}
{
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
	std::vector<CardWidget::Ptr> selfHand;
	//std::vector<CardWidget::Ptr> selfBoard;
	//std::vector<CardWidget::Ptr> opponentBoard;

	// start by removing everything from the current window
	_context.gui->remove(_selfHandLayout);
	_selfHandLayout->removeAllWidgets();

	unsigned i{0U};
	for(const auto& card : _selfHandCards)
	{
		selfHand.push_back(std::make_shared<CardWidget>(_context.client->getCardData(card.id)));
		// each card is associated to their index in the hand
		selfHand.back()->connect("MousePressed", [this, i]()
		{
			useCard(i);
		});
		_selfHandLayout->add(selfHand.back());
		++i;
	}

	auto cardWidth{_cardsLayoutWidth/static_cast<float>(selfHand.size())};
	_selfHandLayout->setSize(_cardsLayoutWidth, (cardWidth/CardGui::widthCard) * CardGui::heightCard);
	_selfHandLayout->setPosition((_width - _cardsLayoutWidth) / 2.f, _height - (_selfHandLayout->getSize().y+10));

	_context.gui->add(_endTurnButton);
	_context.gui->add(_selfHandLayout);

	refreshScreen();
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

		while(_context.window->pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				quit();
			else
				_context.gui->handleEvent(event);
		}
	}
}

GuiGame::~GuiGame()
{
	_context.gui->removeAllWidgets();
}

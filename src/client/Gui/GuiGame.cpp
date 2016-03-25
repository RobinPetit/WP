// WizardPoker headers
#include "client/Gui/GuiGame.hpp"
#include "client/sockets/Client.hpp"
// External headers
#include <TGUI/Widgets/MessageBox.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/VerticalLayout.hpp>

//////////////////// GuiGame

GuiGame::GuiGame(Context& context):
	AbstractGame{*context.client},
	_context{context},
	_decksChosen{false},
	_width{tgui::bindWidth(*_context.gui)},
	_height{tgui::bindHeight(*_context.gui)}
{

}

std::pair<tgui::Layout, tgui::Layout> GuiGame::getSize() const
{
	return {_width, _height};
}

void GuiGame::displayGame()
{
	// TODO
}

// NOTE: this is copy/paste from GuiAbstractState. TODO: factorize by creating an interface
// Messageable, GuiMessageable, TerminalMessageable to factorize this code.
void GuiGame::displayMessage(const std::string& message)
{
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

void GuiGame::chooseDeck()
{
	tgui::VerticalLayout::Ptr decksLayout{std::make_shared<tgui::VerticalLayout>()};
	std::vector<Deck> decks{_client.getDecks()};
	std::vector<DeckWidget> deckWidgets;
	deckWidgets.reserve(decks.size());

	decksLayout->setSize(_width/2.f, _height/2.f);
	decksLayout->setPosition(_width/4.f, _height/4.f);
	for(const auto& deck : decks)
	{
		deckWidgets.emplace_back(DeckWidget(deck, *this, &GuiGame::sendDeck));
		decksLayout->add(deckWidgets.back().getLayout());
	}
	_context.gui->add(decksLayout);
	_context.window->clear(sf::Color::White);
	_context.gui->draw();
	_context.window->display();
	sf::Event event;
	while(not _decksChosen and _context.window->pollEvent(event))
		_context.gui->handleEvent(event);
}

#include <iostream>

void GuiGame::sendDeck(const std::string& deckName)
{
	_decksChosen = true;
	std::cout << "sending " << deckName << std::endl;
}

void GuiGame::receiveCard(cardId id)
{
	// TODO: show the card
	displayMessage("You won the card " + getCardName(id) + ".");
}

//////////////////// DeckWidget

GuiGame::DeckWidget::DeckWidget(const Deck& deck, GuiGame& game, DeckWidget::Callback callback):
	_deckName{std::make_shared<tgui::Label>()},
	_deckButton{std::make_shared<tgui::Button>()},
	_layout{std::make_shared<tgui::HorizontalLayout>()}
{
	auto windowSize{game.getSize()};
	_deckName->setText(deck.getName());
	_deckButton->setText("Send");
	_deckButton->connect("pressed", [this, &game, callback]()
	{
		(game.*callback)(_deckName->getText());
	});
	_layout->setSize(windowSize.first/2.f, 15.f);
	_layout->add(_deckName);
	_layout->add(_deckButton);
}

tgui::HorizontalLayout::Ptr GuiGame::DeckWidget::getLayout()
{
	return _layout;
}

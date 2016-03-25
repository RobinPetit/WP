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
	_height{tgui::bindHeight(*_context.gui)}
{

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
	std::vector<Deck> decks{_client.getDecks()};

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

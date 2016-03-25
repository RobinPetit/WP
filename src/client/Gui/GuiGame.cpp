// WizardPoker headers
#include "client/Gui/GuiGame.hpp"
// External headers
#include <TGUI/Widgets/MessageBox.hpp>
#include <TGUI/Widgets/Button.hpp>

GuiGame::GuiGame(Client& client, tgui::Gui *gui):
	AbstractGame{client},
	_gui{gui}
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
	_gui->add(messageBox);
	messageBox->setPosition((tgui::bindWidth(*_gui) - tgui::bindWidth(messageBox)) / 2,
			(tgui::bindHeight(*_gui) - tgui::bindHeight(messageBox)) / 2);

	// Make the "Ok" button closing the message box
	// Note: do not try to pass messageBox as reference, since this lambda will
	// be stored elsewhere, the reference will become invalid when we'll go out
	// of the scope of this method! We must pass it by value.
	messageBox->connect("buttonPressed", [messageBox](const sf::String& buttonName)
	{
		if(buttonName == okButtonText)
			messageBox->destroy();
	});
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
	displayMessage("Choosing decks");
	// TODO
}

void GuiGame::receiveCard(cardId id)
{
	// TODO: show the card
	displayMessage("You won the card " + getCardName(id) + ".");
}

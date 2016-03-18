// std-C++ headers
#include <iostream>
// WidzardPoker headers
#include "client/Gui/states/GuiMainMenuState.hpp"
#include "client/Gui/states/GuiFriendsManagementState.hpp"

const std::vector<ButtonData<GuiMainMenuState>> GuiMainMenuState::_buttons =
{
	{&GuiMainMenuState::findGame, "Enter the matchmaking lobby"},
	{&GuiMainMenuState::manageDecks, "Manage your decks"},
	{&GuiMainMenuState::seeCards, "See your collection of cards"},
	{&GuiMainMenuState::manageFriends, "Manage your friends"},
	{&GuiMainMenuState::seeLadder, "See the ladder"},
	{&GuiMainMenuState::logOut, "Log out"},
	{&GuiMainMenuState::quit, "Quit"},
};

GuiMainMenuState::GuiMainMenuState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractMainMenuState(context),
	_menuLabel{std::make_shared<tgui::Label>()},
	_layout{std::make_shared<tgui::VerticalLayout>()}
{
	_context.gui->removeAllWidgets();
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically
	// update when the view of the gui changes
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	_menuLabel->setText("Main Menu");
	_menuLabel->setTextSize(30);
	// center the label on the X axis
	_menuLabel->setPosition(windowWidth/2 - tgui::bindWidth(_menuLabel)/2, 40);
	_context.gui->add(_menuLabel);

	_layout->setPosition(windowWidth/5, windowHeight/5);
	_layout->setSize(windowWidth*3/5, windowHeight*3/5);
	for(auto& button : _buttons)
	{
		button.button->setText(button.text);
		button.button->connect("pressed", button.callback, this);
		_layout->add(button.button);
	}
	_context.gui->add(_layout);
}

void GuiMainMenuState::findGame()
{
	// stackPush<GuiLobbyState>();
}

void GuiMainMenuState::manageDecks()
{
	// stackPush<GuiDecksManagementState>();
}

void GuiMainMenuState::seeCards()
{
	// stackPush<GuiCardsCollectionState>();
}

void GuiMainMenuState::manageFriends()
{
	stackPush<GuiFriendsManagementState>();
}

void GuiMainMenuState::seeLadder()
{
	// stackPush<GuiLadderState>();
}

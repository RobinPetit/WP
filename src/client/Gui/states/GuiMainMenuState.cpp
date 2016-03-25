// WidzardPoker headers
#include "client/Gui/states/GuiMainMenuState.hpp"
#include "client/Gui/states/GuiFriendsManagementState.hpp"
#include "client/Gui/states/GuiLadderState.hpp"
#include "client/Gui/states/GuiCardsCollectionState.hpp"
#include "client/Gui/states/GuiDecksManagementState.hpp"
#include "client/Gui/states/GuiLobbyState.hpp"

GuiMainMenuState::GuiMainMenuState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractMainMenuState(context),
	_buttons
	{
		{&GuiMainMenuState::findGame, "Enter the matchmaking lobby"},
		{&GuiMainMenuState::manageDecks, "Manage your decks"},
		{&GuiMainMenuState::seeCards, "See your collection of cards"},
		{&GuiMainMenuState::manageFriends, "Manage your friends"},
		{&GuiMainMenuState::seeLadder, "See the ladder"},
		{&GuiMainMenuState::logOut, "Log out"},
		{&GuiMainMenuState::quit, "Quit"},
	},
	_layout{std::make_shared<tgui::VerticalLayout>()}
{
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically
	// update when the view of the gui changes
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Main Menu");

	_layout->setPosition(windowWidth/5.f, windowHeight/5.f);
	_layout->setSize(windowWidth*3.f/5.f, windowHeight*3.f/5.f);
	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_layout));
	_context.gui->add(_layout);

	registerRootWidgets({_layout});
}

void GuiMainMenuState::findGame()
{
	_context.stateStack->push<GuiLobbyState>();
}

void GuiMainMenuState::manageDecks()
{
	// _context.stateStack->push<GuiDecksManagementState>();
}

void GuiMainMenuState::seeCards()
{
	// _context.stateStack->push<GuiCardsCollectionState>();
}

void GuiMainMenuState::manageFriends()
{
	_context.stateStack->push<GuiFriendsManagementState>();
}

void GuiMainMenuState::seeLadder()
{
	_context.stateStack->push<GuiLadderState>();
}

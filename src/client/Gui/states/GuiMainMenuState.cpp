// std-C++ headers
#include <iostream>
// WidzardPoker headers
#include "client/Gui/states/GuiMainMenuState.hpp"

GuiMainMenuState::GuiMainMenuState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractMainMenuState(context),
	_menuLabel{std::make_shared<tgui::Label>()},
	_lobbyButton{std::make_shared<tgui::Button>()},
	_decksButton{std::make_shared<tgui::Button>()},
	_cardsButton{std::make_shared<tgui::Button>()},
	_friendsButton{std::make_shared<tgui::Button>()},
	_ladderButton{std::make_shared<tgui::Button>()},
	_logOutButton{std::make_shared<tgui::Button>()},
	_quitButton{std::make_shared<tgui::Button>()},
	_grid{std::make_shared<tgui::Grid>()}
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

	_lobbyButton->setText("Enter the matchmaking lobby");
	_decksButton->setText("Manage your decks");
	_cardsButton->setText("See your collection of cards");
	_friendsButton->setText("Manage your friends");
	_ladderButton->setText("See the ladder");
	_logOutButton->setText("Log out");
	_quitButton->setText("Quit");

	_lobbyButton->connect("pressed", &GuiMainMenuState::findGame, this);
	_decksButton->connect("pressed", &GuiMainMenuState::manageDecks, this);
	_cardsButton->connect("pressed", &GuiMainMenuState::seeCards, this);
	_friendsButton->connect("pressed", &GuiMainMenuState::manageFriends, this);
	_ladderButton->connect("pressed", &GuiMainMenuState::seeLadder, this);
	_logOutButton->connect("pressed", &GuiMainMenuState::logOut, this);
	_quitButton->connect("pressed", &GuiMainMenuState::quit, this);

	_grid->setPosition(windowWidth/5, windowHeight/5);
	_grid->setSize(windowWidth*3/5, windowHeight*3/5);
	_grid->addWidget(_lobbyButton, 0, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_decksButton, 1, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_cardsButton, 2, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_friendsButton, 3, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_ladderButton, 4, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_logOutButton, 5, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_quitButton, 6, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_context.gui->add(_grid);
}

void GuiMainMenuState::findGame()
{
	// stackPush<GuiLobyState>();
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
	// stackPush<GuiFriendsManagementState>();
}

void GuiMainMenuState::seeLadder()
{
	// stackPush<GuiLadderState>();
}

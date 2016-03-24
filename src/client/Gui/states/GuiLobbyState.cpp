// WizardPoker headers
#include "client/Gui/states/GuiLobbyState.hpp"
#include "client/sockets/Client.hpp"
// std-C++ headers
#include <chrono>

GuiLobbyState::GuiLobbyState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractLobbyState(context),
	_buttons
	{
		{&GuiLobbyState::findAGame, "Find a game"},
		{&GuiLobbyState::quit, "Back to previous menu"},
	},
	_cancelButton{std::make_shared<tgui::Button>()},
	_menuLabel{std::make_shared<tgui::Label>()},
	_buttonsLayout{std::make_shared<tgui::VerticalLayout>()},
	_play{false}
{
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically
	// update when the view of the gui changes
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	_menuLabel->setText("Welcome to the Lobby Menu");
	_menuLabel->setTextSize(30);
	// center the label on the X axis
	_menuLabel->setPosition(windowWidth/2 - tgui::bindWidth(_menuLabel)/2, 40);
	_context.gui->add(_menuLabel);

	_buttonsLayout->setPosition(windowWidth/5.f, windowHeight/5.f);
	_buttonsLayout->setSize(windowWidth*3.f/5.f, windowHeight*3.f/5.f);
	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_buttonsLayout));

	_cancelButton->setText("Cancel");
	_cancelButton->setPosition(windowWidth/5.f, windowHeight*3.f/5.f);
	_cancelButton->setSize(windowWidth/5.f, windowHeight/5.f);
	_cancelButton->connect("pressed", [this]()
	{
		_context.client->leaveLobby();
		displayMessage("You quitted the lobby");
		resetButtons();
		_play = false;
	});
	_cancelButton->hide();
	_buttonsLayout->add(_cancelButton);
	_context.gui->add(_buttonsLayout);

	registerRootWidgets({_menuLabel, _buttonsLayout});
}

void GuiLobbyState::findAGame()
{
	if(not _context.client->isConnected())
	{
		displayMessage("You can't play: you're not connected");
		return;
	}
	_play = true;
	_context.client->enterLobby();
	setButtonsAsWaiting();
	std::string opponentName;
	while(_play and not _context.client->isGameStarted(opponentName))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		handleInput();
	}
	resetButtons();
	if(_play)
		displayMessage("Let's start a game with " + opponentName);
}

void GuiLobbyState::resetButtons()
{
	for(auto& button : _buttons)
		button.button->show();
	_cancelButton->hide();
	display();
}

void GuiLobbyState::setButtonsAsWaiting()
{
	for(auto& button : _buttons)
		button.button->hide();
	_cancelButton->show();
	display();
}

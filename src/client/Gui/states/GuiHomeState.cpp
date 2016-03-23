// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "common/UnableToConnectException.hpp"
#include "client/Gui/states/GuiMainMenuState.hpp"
#include "client/Gui/states/GuiHomeState.hpp"

GuiHomeState::GuiHomeState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractHomeState(context),
	_titleLabel{std::make_shared<tgui::Label>()},
	_userNameLabel{std::make_shared<tgui::Label>()},
	_passwordLabel{std::make_shared<tgui::Label>()},
	_userNameEditBox{std::make_shared<tgui::EditBox>()},
	_passwordEditBox{std::make_shared<tgui::EditBox>()},
	_connectButton{std::make_shared<tgui::Button>()},
	_createAccountButton{std::make_shared<tgui::Button>()},
	_grid{std::make_shared<tgui::Grid>()}
{
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically
	// update when the view of the gui changes
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	// Make the title
	_titleLabel->setText("Wizard Poker");
	_titleLabel->setTextSize(40);
	_titleLabel->setPosition(windowWidth / 2 - tgui::bindWidth(_titleLabel) / 2, 60);
	_context.gui->add(_titleLabel);

	// Configure the widgets
	_userNameLabel->setText("Username:");
	_passwordLabel->setText("Password:");
	_passwordEditBox->setPasswordCharacter('*');  // Display only stars in the edit box
	_connectButton->setText("Connect");
	_connectButton->connect("pressed", &GuiHomeState::connect, this);
	_createAccountButton->setText("Create an account");
	_createAccountButton->connect("pressed", &GuiHomeState::createAccount, this);

	// Add the widgets to the grid
	_grid->setPosition(windowWidth / 4, windowHeight / 4);
	_grid->setSize(windowWidth / 2, windowHeight / 2);
	_grid->addWidget(_userNameLabel, 0, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_userNameEditBox, 0, 1, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_passwordLabel, 1, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_passwordEditBox, 1, 1, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	_grid->addWidget(_connectButton, 2, 0);
	_grid->addWidget(_createAccountButton, 2, 1);
	_context.gui->add(_grid);

	registerRootWidgets({_grid, _titleLabel});
}

void GuiHomeState::connect()
{
	try
	{
		auto connectionConfig(getConnectionConfiguration());
		const std::string userName{_userNameEditBox->getText()};
		const std::string password{_passwordEditBox->getText()};
		// The following code is created to handle the case of a crash during
		// server execution and then finding an available port to connect to
		// (associated code in the server)
		bool tryToConnect{true};
		int counter{0};
		while(tryToConnect)
		{
			try
			{
				_context.client->connectToServer(userName, password,
				                        connectionConfig.first, connectionConfig.second);
				tryToConnect = false;
			}
			catch(const UnableToConnectException& e)
			{
				tryToConnect = (++counter) <= 10;
				++connectionConfig.second;
				// Manually break by rethrowing the exception e
				if(not tryToConnect)
					throw;
			}
		}
		_context.stateStack->push<GuiMainMenuState>();
	}
	catch(const std::runtime_error& e)
	{
		displayMessage(e.what());
		// If the error was caused by client::updateFriends, the client is connected
		// but the user will stay on this state. The client must be deconnected.
		_context.client->quit();
	}
}

void GuiHomeState::createAccount()
{
	try
	{
		const auto connectionConfig(getConnectionConfiguration());
		const std::string userName{_userNameEditBox->getText()};
		const std::string password{_passwordEditBox->getText()};
		_context.client->registerToServer(userName, password,
		                         connectionConfig.first, connectionConfig.second);
	}
	catch(const std::runtime_error& e)
	{
		displayMessage(e.what());
	}
}

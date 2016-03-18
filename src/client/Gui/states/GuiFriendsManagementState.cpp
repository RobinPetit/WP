// WizardPoker headers
#include "client/Gui/states/GuiFriendsManagementState.hpp"

const std::vector<ButtonData<GuiFriendsManagementState>> GuiFriendsManagementState::_buttons =
{
	{&GuiFriendsManagementState::backMainMenu, "Back to main menu"},
	{&GuiFriendsManagementState::addFriend, "Add a friend to the list"},
	{&GuiFriendsManagementState::removeFriend, "Remove a friend from the list"},
	{&GuiFriendsManagementState::treatRequests, "Treat friendship requests"},
	{&GuiFriendsManagementState::startChat, "Chat with a friend"},
};

GuiFriendsManagementState::GuiFriendsManagementState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractFriendsManagementState(context),
	_menuLabel{std::make_shared<tgui::Label>()},
	_grid{std::make_shared<tgui::Grid>()}
{
	_context.gui->removeAllWidgets();
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically
	// update when the view of the gui changes
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));
	_menuLabel->setText("Friends anagement menu");
	_menuLabel->setTextSize(30);
	// center the label on the X axis
	_menuLabel->setPosition(windowWidth/2 - tgui::bindWidth(_menuLabel)/2, 40);
	_context.gui->add(_menuLabel);

	_grid->setPosition(windowWidth/5,windowHeight/5);
	_grid->setSize(windowWidth*3/4, windowHeight*3/4);
	int i{0};
	for(auto& button : _buttons)
	{
		button.button->setText(button.text);
		button.button->connect("pressed", button.callback, this);
		_grid->addWidget(button.button, i++, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::Up);
	}
	_context.gui->add(_grid);
}

void GuiFriendsManagementState::addFriend()
{
	;
}

void GuiFriendsManagementState::removeFriend()
{
	;
}

void GuiFriendsManagementState::treatRequests()
{
	;
}

void GuiFriendsManagementState::startChat()
{
	;
}

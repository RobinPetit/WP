// WizardPoker headers
#include "client/Gui/states/GuiFriendsManagementState.hpp"

const std::vector<ButtonData<GuiFriendsManagementState>> GuiFriendsManagementState::_buttons =
{
	{&GuiFriendsManagementState::addFriend, "Add a friend to the list"},
	{&GuiFriendsManagementState::removeFriend, "Remove selected friend"},
	{&GuiFriendsManagementState::treatRequests, "Treat friendship requests"},
	{&GuiFriendsManagementState::startChat, "Chat with selected friend"},
	{&GuiFriendsManagementState::backMainMenu, "Back to main menu"},
};

GuiFriendsManagementState::GuiFriendsManagementState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractFriendsManagementState(context),
	_menuLabel{std::make_shared<tgui::Label>()},
	_friendsListBox{std::make_shared<tgui::ListBox>()},
	_buttonsLayout{std::make_shared<tgui::VerticalLayout>()}
{
	_context.gui->removeAllWidgets();
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically
	// update when the view of the gui changes
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	// Make the label
	_menuLabel->setText("Friends anagement menu");
	_menuLabel->setTextSize(30);
	// center the label on the X axis
	_menuLabel->setPosition(windowWidth/2.f - tgui::bindWidth(_menuLabel)/2.f, 40);
	_context.gui->add(_menuLabel);

	// Make the buttons
	_buttonsLayout->setPosition(windowWidth/2.f, windowHeight/5.f);
	_buttonsLayout->setSize(windowWidth*(1.f/2.f - 1.f/5.f), windowHeight*3.f/4.f);
	for(auto& button : _buttons)
	{
		button.button->setText(button.text);
		button.button->connect("pressed", button.callback, this);
		_buttonsLayout->add(button.button);
	}
	_context.gui->add(_buttonsLayout);

	// Make the friends list box
	_friendsListBox->setPosition(windowWidth/5, windowHeight/5);
	_friendsListBox->setSize(windowWidth*(1.f/2.f - 1.f/5.f), windowHeight*3.f/4.f);
	updateFriendListBox();
	_context.gui->add(_friendsListBox);
}

void GuiFriendsManagementState::addFriend()
{
	;
}

void GuiFriendsManagementState::removeFriend()
{
	if(_friendsListBox->getSelectedItemIndex() != -1)
		_context.client->removeFriend(_friendsListBox->getSelectedItem());
	updateFriendListBox();
}

void GuiFriendsManagementState::treatRequests()
{
	;
}

void GuiFriendsManagementState::startChat()
{
	if(_friendsListBox->getSelectedItemIndex() != -1)
		_context.client->startConversation(_friendsListBox->getSelectedItem());
}

void GuiFriendsManagementState::updateFriendListBox()
{
	_friendsListBox->removeAllItems();
	for(const auto& friendUser : _context.client->getFriends())
		_friendsListBox->addItem(friendUser.name);
}

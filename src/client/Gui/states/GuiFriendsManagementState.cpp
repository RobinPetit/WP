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
	_menuLabel->setText("Friends management menu");
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
	for(const auto& friendUser : _context.client->getFriends())
		_friendsListBox->addItem(friendUser.name);
	_context.gui->add(_friendsListBox);
}

void GuiFriendsManagementState::addFriend()
{
	;
}

void GuiFriendsManagementState::removeFriend()
{
	std::string selectedFriend{_friendsListBox->getSelectedItem()};
	if(selectedFriend == "")
	{
		displayMessage("You need to select a friend first");
		return;
	}
	try
	{
		_context.client->removeFriend(selectedFriend);
		if(not _friendsListBox->removeItem(selectedFriend))
			throw std::runtime_error("Unable to remove friend from the list box");
	}
	catch(std::runtime_error& e)
	{
		displayMessage(e.what());
	}
}

void GuiFriendsManagementState::treatIndividualRequest(const Friend& friendRequest)
{
	const static std::string acceptButton{"Accept"};
	const static std::string refuseButton{"Refuse"};
	const static std::string ignoreButton{"Ignore"};
	tgui::MessageBox::Ptr messageBox{std::make_shared<tgui::MessageBox>()};

	// warn that a player wants to become friend
	messageBox->setText(friendRequest.name + " wants to become your friend");

	// Add the three possible buttons
	messageBox->addButton(acceptButton);
	messageBox->addButton(refuseButton);
	messageBox->addButton(ignoreButton);

	_context.gui->add(messageBox);
	// center messageBox on current window
	messageBox->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(messageBox)) / 2,
			(tgui::bindHeight(*_context.gui) - tgui::bindHeight(messageBox)) / 2);

	messageBox->connect("buttonPressed", [messageBox, friendRequest, this](const std::string& pressedButtonName)
	{
		// accept or refuse according to what button's been pressed
		if(pressedButtonName == acceptButton)
		{
			// send to server that the request is accepted
			_context.client->acceptFriendshipRequest(friendRequest.name);
			// and add the friend to the list box
			_friendsListBox->addItem(friendRequest.name);
		}
		else if(pressedButtonName == refuseButton)
			_context.client->acceptFriendshipRequest(friendRequest.name, false);
		// destroy the message box anyway
		messageBox->destroy();
	});
}

void GuiFriendsManagementState::startChat()
{
	;
}

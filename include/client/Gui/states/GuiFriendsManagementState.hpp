#ifndef _GUI_FRIENDS_MANAGEMENT_STATE_HPP
#define _GUI_FRIENDS_MANAGEMENT_STATE_HPP_

// External headers
#include "TGUI/Widgets/Grid.hpp"
// WizardPoker headers
#include "client/Gui/states/ButtonData.hpp"
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractFriendsManagementState.hpp"
// std-C++ headers
#include <vector>

class GuiFriendsManagementState : public GuiAbstractState, public AbstractFriendsManagementState
{
public:
	/// Constructor
	GuiFriendsManagementState(Context& context);

private:
	////////// Attributes
	static const std::vector<ButtonData<GuiFriendsManagementState>> _buttons;

	tgui::Label::Ptr _menuLabel;

	tgui::Grid::Ptr _grid;

	////////// Methdos

	void addFriend();
	void removeFriend();
	void treatRequests();
	void startChat();

};

#endif  // _GUI_FRIENDS_MANAGEMENT_STATE_HPP_

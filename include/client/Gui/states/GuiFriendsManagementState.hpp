#ifndef _GUI_FRIENDS_MANAGEMENT_STATE_HPP
#define _GUI_FRIENDS_MANAGEMENT_STATE_HPP_

// External headers
#include <TGUI/VerticalLayout.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/MessageBox.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractFriendsManagementState.hpp"
// std-C++ headers
#include <vector>

/// Final class for friends management with the GUI.
class GuiFriendsManagementState : public GuiAbstractState, public AbstractFriendsManagementState
{
	public:
		/// Constructor
		GuiFriendsManagementState(Context& context);

	private:
		////////// Attributes
		const std::vector<ButtonData<GuiFriendsManagementState>> _buttons;

		tgui::Label::Ptr _menuLabel;
		tgui::ListBox::Ptr _friendsListBox;
		tgui::VerticalLayout::Ptr _buttonsLayout;

		////////// Methdos

		void addFriend();
		void removeFriend();
		void startChat();
		void updateFriendListBox();

		void treatIndividualRequest(const Friend& friendRequest) override;
};

#endif  // _GUI_FRIENDS_MANAGEMENT_STATE_HPP_

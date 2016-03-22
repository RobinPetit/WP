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

class GuiFriendsManagementState : public GuiAbstractState, public AbstractFriendsManagementState
{
public:
	/// Constructor
	GuiFriendsManagementState(Context& context);

	/// Destructor. Deletes all widgets of this state.
	~GuiFriendsManagementState();

	/// Method called when another state is pushed on this one.
	/// By default, does nothing.
	/// Hides all the widgets.
	virtual void onPush() override;

	/// Method called when this state become the TOS again (e.g. this method
	/// is called on the HomeState instance when we log out from the main
	/// menu state).
	/// Shows all the widgets again.
	virtual void onPop() override;

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

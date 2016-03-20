#ifndef _GUI_MAIN_MENU_STATE_CLIENT_HPP
#define _GUI_MAIN_MENU_STATE_CLIENT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractMainMenuState.hpp"

/// Main menu in GUI Interface
class GuiMainMenuState: public GuiAbstractState, public AbstractMainMenuState
{
	public:
		/// Constructor
		GuiMainMenuState(Context& context);

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
		tgui::Label::Ptr _menuLabel;

		tgui::VerticalLayout::Ptr _layout;

		const static std::vector<ButtonData<GuiMainMenuState>> _buttons;

		////////// Methods
		void findGame();
		void manageDecks();
		void seeCards();
		void manageFriends();
		void seeLadder();
};

#endif  // _GUI_MAIN_MENU_STATE_CLIENT_HPP

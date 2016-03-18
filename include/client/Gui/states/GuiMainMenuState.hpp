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

	private:
		////////// Attributes
		tgui::Label::Ptr _menuLabel;
		tgui::Button::Ptr _lobbyButton;
		tgui::Button::Ptr _decksButton;
		tgui::Button::Ptr _cardsButton;
		tgui::Button::Ptr _friendsButton;
		tgui::Button::Ptr _ladderButton;
		tgui::Button::Ptr _logOutButton;
		tgui::Button::Ptr _quitButton;

		tgui::Grid::Ptr _grid;

		////////// Methods
		void findGame();
		void manageDecks();
		void seeCards();
		void manageFriends();
		void seeLadder();
};

#endif // _GUI_MAIN_MENU_STATE_CLIENT_HPP

#ifndef _GUI_LADDER_STATE_CLIENT_HPP
#define _GUI_LADDER_STATE_CLIENT_HPP

// External headers
#include <TGUI/HorizontalLayout.hpp>
#include <TGUI/VerticalLayout.hpp>
#include <TGUI/Animation.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>

// WizardPoker headers
#include "common/Database.hpp"  // Ladder
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractLadderState.hpp"


/// Final class for the ladder with GUI.
class GuiLadderState : public GuiAbstractState, public AbstractLadderState
{
	public:
		/// Constructor.
		GuiLadderState(Context& context);

	private:
		const std::vector<ButtonData<GuiLadderState>> _buttons;

		/// A structure that contains all widgets needed for displaying an entry
		/// of the ladder in GUI.
		struct GuiLadderEntry
		{
			/// Constructor.
			GuiLadderEntry();

			/// Set the color of the labels
            void setBackgroundColor(const sf::Color&);

			/// The rank field (from 1 to _ladder.size()).
			tgui::Label::Ptr rankLabel;

			/// The name of the player.
			tgui::Label::Ptr playerNameLabel;

			/// The number of games the player won.
			tgui::Label::Ptr wonGamesLabel;

			/// The number of games the player played.
			tgui::Label::Ptr playedGamesLabel;

			tgui::Label::Ptr ratioLabel;

			/// The layoud to ties all labels together.
			tgui::HorizontalLayout::Ptr layout;
		};

		/// The list of all lines of the ladder that will be displayed.
		//std::array<GuiLadderEntry, std::tuple_size<decltype(_ladder)>::value> _guiLadder;
		std::vector<GuiLadderEntry> _guiLadder;

		/// The first line of the ladder (the top line with the names of the
		/// columns).
		GuiLadderEntry _ladderHeader;

		/// Panel contains the layout and the scrollbar
		tgui::Panel::Ptr _panel;
		tgui::Scrollbar::Ptr _scrollbar;
		tgui::VerticalLayout::Ptr _ladderLayout;

		void scrollGrid(int newScrollValue);
};

#endif  // _GUI_LADDER_STATE_CLIENT_HPP

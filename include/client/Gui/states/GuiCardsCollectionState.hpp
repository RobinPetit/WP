#ifndef _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP
#define _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP

// External headers
#include <TGUI/Widgets/Grid.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractCardsCollectionState.hpp"
#include "client/Gui/CardWidget.hpp"

/// Final class for the cards collection with the GUI.
class GuiCardsCollectionState : public GuiAbstractState, public AbstractCardsCollectionState
{
	public:
		/// Constructor.
		GuiCardsCollectionState(Context& context);

	private:
		/// The type is unsigned rather than size_t because this is the type
		/// used by TGUI.
		static constexpr unsigned int GRID_WIDTH{3};
		const std::vector<ButtonData<GuiCardsCollectionState>> _buttons;
		tgui::Grid::Ptr _cardGrid;
		tgui::Panel::Ptr _gridPanel;
		tgui::Scrollbar::Ptr _scrollbar;
		std::vector<CardWidget::Ptr> _cards;

		void scrollGrid(int newScrollValue);
};

#endif  // _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP

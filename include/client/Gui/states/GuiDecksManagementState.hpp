#ifndef _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP

// External headers
#include <TGUI/VerticalLayout.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/Grid.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>
#include <TGUI/Widgets/Label.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractDecksManagementState.hpp"
#include "client/Gui/CardWidget.hpp"
#include "client/Gui/states/ButtonData.hpp"
#include "common/Identifiers.hpp"

/// Final class for decks management with the GUI.
class GuiDecksManagementState : public GuiAbstractState, public AbstractDecksManagementState
{
	public:
		/// Constructor.
		GuiDecksManagementState(Context& context);

	private:
		const std::vector<ButtonData<GuiDecksManagementState>> _buttons;
		tgui::VerticalLayout::Ptr _buttonsLayout;
		tgui::ListBox::Ptr _decksListBox;
		tgui::Grid::Ptr _cardGrid;
		tgui::Panel::Ptr _cardPanel;
		tgui::Scrollbar::Ptr _scrollbar;
		std::vector<std::pair<cardId, CardWidget::Ptr>> _cardsCollection;
		tgui::Label::Ptr _hintLabel;

		/// This variable determines the state of the current operation: if
		/// true, the deck is displayed in the grid and we are waiting for the
		/// user to click on a card, and if false, the card collection is
		/// displayed and the clicked card will replace the one previously
		/// clicked when this variable was true.
		bool chooseCardFromDeck;

		static constexpr std::size_t GRID_WIDTH = 2;

		static constexpr float PADDING{30.f};

	private:
		/// Callback called when the user clicks on a card.
		void onCardClick(cardId id);

		void selectDeck();

		void removeDeck();

		void createDeck();
};

#endif  // _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP

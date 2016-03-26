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
/// The menu works as follow:
/// A list of the decks is displayed in a side, and in the other side a
/// scrollable grid with the cards of the selected deck is
/// displayed.
///
/// To change the content of a deck, the user has to click on a card
/// that he wants to replace (after selecting a deck in the list of course), and
/// once he clicked the grid content is replaced with its card collection.
///
/// He has now to click on a card in the grid (which now represents its
/// collection, and the two clicked cards will be swapped.
///
/// In the code of this class, the term "selected deck" designates a deck (so
/// representated by a member of _decks) whose name matches with the selected
/// item in _decksListBox.
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

		/// A list of widgets for all cards of the collection, not all of them
		/// are displayed at the same time, it depends on the selected deck
		/// (if there is one).
		std::vector<CardWidget::Ptr> _cardsCollectionWidgets;

		/// And the IDs associated with the widgets. We need this vector rather
		/// than just using AbstractCardsCollectionState::_cardsCollection
		/// because the latter is not ordered.
		std::vector<CardId> _cardsCollectionIds;

		/// A label that display an hint to the user to help him in the deck
		/// management process.
		tgui::Label::Ptr _hintLabel;

		/// This variable determines the state of the current operation: if
		/// true, the deck is displayed in the grid and we are waiting for the
		/// user to click on a card, and if false, the card collection is
		/// displayed and the clicked card will replace the one previously
		/// clicked when this variable was true.
		bool _chooseCardFromDeck;

		/// If _chooseCardFromDeck is false, this is the card selected in the
		/// deck previously.
		CardId _selectedCardFromDeck;

		static constexpr unsigned int GRID_WIDTH = 2;

		/// The space between the content and the window limits.
		static constexpr float PADDING{30.f};

		/// Hint that is displayed when no deck is selected.
		static constexpr char FIRST_HINT[] = "First, select a deck in the list";

		/// Hint that is displayed when the user has to choose a card from the
		/// selected deck.
		static constexpr char CHOOSE_CARD_FROM_DECK_HINT[] = "This is the content of you deck. Click on a card that you want to replace";

		/// Hint that is displayed when the user has to choose a card from its
		/// collection.
		static constexpr char CHOOSE_CARD_FROM_COLLECTION_HINT[] = "This is the content of your collection. Click on a card that you want to put in your deck";

		/// Callback called when the user clicks on a card.
		void onCardClicked(CardId id);

		/// Called by onCardClicked if we clicked on a card in the selected deck.
		void onCardChosenFromDeck(CardId id);

		/// Called by onCardClicked if we clicked on a card in the collection.
		void onCardChosenFromCollection(CardId id);

		/// Populates _cardGrid with the given widgets.
		void fillGrid(const std::vector<CardWidget::Ptr>& widgetsToShow);

		/// Callback called when the user select another deck. Effectively
		/// change the content of the grid to reflect the content of the newly
		/// selected deck.
		void selectDeck();

		/// Callback called when the user asks for deleting the selected deck.
		void removeDeck();

		/// Callback called when the user asks for creating a new deck.
		void createDeck();

		/// Gives a reference to the currently selected deck.
		Deck& getSelectedDeck();

		/// Callback called when the user scrolls the grid.
		void scrollGrid(int newScrollValue);
};

#endif  // _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP

#ifndef _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP
#define _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractCardsCollectionState.hpp"

/// Final class for the cards collection with the GUI.
class GuiCardsCollectionState : public GuiAbstractState, public AbstractCardsCollectionState
{
	public:
		/// Constructor.
		GuiCardsCollectionState(Context& context);
};

#endif  // _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP

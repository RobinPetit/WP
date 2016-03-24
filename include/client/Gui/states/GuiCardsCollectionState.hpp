#ifndef _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP
#define _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractCardsCollectionState.hpp"
#include "client/Gui/CreatureGui.hpp"
#include "client/Gui/SpellGui.hpp"

/// Final class for the cards collection with the GUI.
class GuiCardsCollectionState : public GuiAbstractState, public AbstractCardsCollectionState
{
	public:
		/// Constructor.
		GuiCardsCollectionState(Context& context);

	private:
		const std::vector<ButtonData<GuiCardsCollectionState>> _buttons;
		tgui::Label::Ptr _titleLabel;
};

#endif  // _GUI_CARDS_COLLECTION_STATE_CLIENT_HPP

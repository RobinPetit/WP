// WizardPoker headers
#include "client/Gui/states/GuiCardsCollectionState.hpp"
#include "client/Gui/CreatureGui.hpp"

GuiCardsCollectionState::GuiCardsCollectionState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractCardsCollectionState(context),
	_buttons
	{
		{&GuiCardsCollectionState::backMainMenu, "Back to main menu"}
	}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Cards collection");

	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_context.gui->getContainer()));
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight * 8.f/10.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, windowHeight * 1/10.f);

	registerRootWidgets({_buttons[0].button});

}

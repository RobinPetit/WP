// WizardPoker headers
#include "client/Gui/states/GuiCardsCollectionState.hpp"

GuiCardsCollectionState::GuiCardsCollectionState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractCardsCollectionState(context),
	_buttons
	{
		{&GuiCardsCollectionState::backMainMenu, "Back to main menu"}
	},
	_titleLabel{std::make_shared<tgui::Label>()},
	_testCreature{"Mecha Spider", "Description here", 3, 4, 10, 1, SHIELD_BLUE},
	_creatureCanvas{std::make_shared<tgui::Canvas>()},
	_testSpell{"Instant heal", "Description here", 10},
	_spellCanvas{std::make_shared<tgui::Canvas>()}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	_titleLabel->setText("Cards collection");
	_titleLabel->setTextSize(30);
	// center the label on the X axis
	_titleLabel->setPosition(windowWidth/2.f - tgui::bindWidth(_titleLabel)/2.f, 40);
	_context.gui->add(_titleLabel);

	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_context.gui->getContainer()));
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight * 8.f/10.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, windowHeight * 1/10.f);

	_creatureCanvas->setPosition(0, 0);
	_creatureCanvas->setSize(400, 400);
	_creatureCanvas->clear(sf::Color::Transparent);
	_creatureCanvas->draw(_testCreature);
	_creatureCanvas->display();
	_context.gui->add(_creatureCanvas);

	_spellCanvas->setPosition(400, 0);
	_spellCanvas->setSize(400, 400);
	_spellCanvas->clear(sf::Color::Transparent);
	_testSpell.setShownSide(false);
	_spellCanvas->draw(_testSpell);
	_spellCanvas->display();
	_context.gui->add(_spellCanvas);

	registerRootWidgets({_titleLabel, _buttons[0].button, _creatureCanvas, _spellCanvas});
}

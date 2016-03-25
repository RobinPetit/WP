#include "client/Gui/states/GuiDecksManagementState.hpp"

GuiDecksManagementState::GuiDecksManagementState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractDecksManagementState(context),
	_buttons
	{
		{&GuiDecksManagementState::createDeck, "Create a new deck"},
		{&GuiDecksManagementState::removeDeck, "Remove the selected deck"},
		{&GuiDecksManagementState::backMainMenu, "Back to main menu"}
	},
	_buttonsLayout{std::make_shared<tgui::VerticalLayout>()},
	_decksListBox{std::make_shared<tgui::ListBox>()},
	_cardGrid{std::make_shared<tgui::Grid>()},
	_cardPanel{std::make_shared<tgui::Panel>()},
	_scrollbar{std::make_shared<tgui::Scrollbar>()},
	_cardsCollection{},
	_hintLabel{std::make_shared<tgui::Label>()},
	chooseCardFromDeck{true}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Decks management");

	_cardPanel->setSize(GRID_WIDTH * CardGui::getSize().x, windowHeight - 120.f);
	_cardPanel->setPosition(windowWidth - tgui::bindWidth(_cardPanel) - PADDING, 100.f);
	_cardPanel->setBackgroundColor(sf::Color::Transparent);
	_context.gui->add(_cardPanel);

	_decksListBox->setPosition(PADDING, 100.f);
	_decksListBox->setSize(windowWidth - tgui::bindWidth(_cardPanel) - 2.f * PADDING, windowHeight * 0.6f - 120.f);
	_context.gui->add(_decksListBox);
	for(const auto& deck : _decks)
		_decksListBox->addItem(deck.getName());

	_buttonsLayout->setPosition(PADDING, tgui::bindBottom(_decksListBox));
	_buttonsLayout->setSize(tgui::bindWidth(_decksListBox), windowHeight - tgui::bindHeight(_decksListBox) - 120.f);
	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_buttonsLayout));
	_context.gui->add(_buttonsLayout);

	registerRootWidgets({_cardPanel, _decksListBox, _buttonsLayout});
}

void GuiDecksManagementState::onCardClick(cardId id)
{
}

void GuiDecksManagementState::selectDeck()
{
}

void GuiDecksManagementState::removeDeck()
{
}

void GuiDecksManagementState::createDeck()
{
}

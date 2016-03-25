// std-C++ headers
#include <algorithm>

#include <iostream>
// WizardPoker headers
#include "client/Gui/states/GuiDecksManagementState.hpp"
#include "client/sockets/Client.hpp"

constexpr char GuiDecksManagementState::FIRST_HINT[];
constexpr char GuiDecksManagementState::CHOOSE_CARD_FROM_DECK_HINT[];
constexpr char GuiDecksManagementState::CHOOSE_CARD_FROM_COLLECTION_HINT[];

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
	_cardsCollectionWidgets{},
	_cardsCollectionIds{},
	_hintLabel{std::make_shared<tgui::Label>()},
	chooseCardFromDeck{true}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Decks management", 30U, 30.f);

	// Make the hint label
	_hintLabel->setText(FIRST_HINT);
	_hintLabel->setTextSize(20U);
	_hintLabel->setPosition((windowWidth - tgui::bindWidth(_hintLabel)) / 2.f, 70);
	_context.gui->add(_hintLabel);

	// Make the card panel
	_cardPanel->setSize(GRID_WIDTH * CardGui::getSize().x, windowHeight - 120.f);
	_cardPanel->setPosition(windowWidth - tgui::bindWidth(_cardPanel) - PADDING, 100.f);
	_cardPanel->setBackgroundColor(sf::Color::Transparent);
	_context.gui->add(_cardPanel);

	// Make the card grid
	_cardGrid->setPosition(0.f, 0.f);
	// Size is set in callback methods
	_cardPanel->add(_cardGrid);

	// Make the scrollbar
	_scrollbar->setPosition(tgui::bindRight(_cardPanel), tgui::bindTop(_cardPanel));
	_scrollbar->setSize((windowWidth - tgui::bindRight(_cardPanel)) / 2.f, tgui::bindHeight(_cardPanel));
	_scrollbar->setLowValue(static_cast<unsigned int>(_cardPanel->getSize().y));
	// Maximum value is set in the callback methods
	_scrollbar->setArrowScrollAmount(30);
	_scrollbar->connect("ValueChanged", &GuiDecksManagementState::scrollGrid, this);
	_context.gui->add(_scrollbar);

	// Make the deck list box
	_decksListBox->setPosition(PADDING, 100.f);
	_decksListBox->setSize(windowWidth - tgui::bindWidth(_cardPanel) - 2.f * PADDING, windowHeight * 0.6f - 120.f);
	for(const auto& deck : _decks)
		_decksListBox->addItem(deck.getName());
	_decksListBox->connect("ItemSelected", &GuiDecksManagementState::selectDeck, this);
	_context.gui->add(_decksListBox);

	// Make the cards collection attributes
	for(auto card : _cardsCollection)
	{
		_cardsCollectionWidgets.push_back(std::make_shared<CardWidget>(_context.client->getCardData(card)));
		_cardsCollectionIds.push_back(card);
		_cardsCollectionWidgets.back()->connect("Clicked", &GuiDecksManagementState::onCardClicked, this, card);
	}

	// Make the buttons layout
	_buttonsLayout->setPosition(PADDING, tgui::bindBottom(_decksListBox));
	_buttonsLayout->setSize(tgui::bindWidth(_decksListBox), windowHeight - tgui::bindHeight(_decksListBox) - 120.f);
	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_buttonsLayout));
	_context.gui->add(_buttonsLayout);

	registerRootWidgets({_cardPanel, _decksListBox, _buttonsLayout, _scrollbar, _hintLabel});
}

void GuiDecksManagementState::onCardClicked(cardId id)
{
	if(chooseCardFromDeck)
		onCardChosenFromDeck(id);
	else
		onCardChosenFromCollection(id);
	chooseCardFromDeck = not chooseCardFromDeck;
}

void GuiDecksManagementState::onCardChosenFromDeck(cardId id)
{
	// Update the hint
	_hintLabel->setText(CHOOSE_CARD_FROM_COLLECTION_HINT);
	_hintLabel->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(_hintLabel)) / 2.f, 70);

	// The user cannot change the selected deck when it has to choose a card
	// from the collection
	_decksListBox->disable();

	Deck& selectedDeck(_decks.at(_decksListBox->getSelectedItemIndex()));

	// Compute the card collection without the cards of the selected deck
	// For these lines, we just care about IDs
	std::vector<cardId> cardIdsToShow;
	std::set_difference(_cardsCollectionIds.begin(), _cardsCollectionIds.end(),
	                    selectedDeck.begin(), selectedDeck.end(),
	                    std::back_inserter(cardIdsToShow));

	// Create the corresponding vector of widgets
	std::vector<CardWidget::Ptr> widgetsToShow;

	// Browse all cards of the collection
	for(std::size_t i{0}; i < _cardsCollectionWidgets.size(); ++i)
	{
		// If the card is also in the collection to show
		if(std::find(cardIdsToShow.begin(), cardIdsToShow.end(), _cardsCollectionIds.at(i)) != cardIdsToShow.end())
			// Add the corresponding widget to the list of widgets to show
			widgetsToShow.push_back(_cardsCollectionWidgets.at(i));
	}

	// Eventually, fill the grid with the computed card collection
	fillGrid(widgetsToShow);

	// TODO: remember which card was selected
}

void GuiDecksManagementState::onCardChosenFromCollection(cardId id)
{
	_decksListBox->enable();

	// Show the current deck, this is the same as when we select another deck
	selectDeck();

	// TODO: effectively swap cards in the selected deck
}

void GuiDecksManagementState::fillGrid(const std::vector<CardWidget::Ptr>& widgetsToShow)
{
	_cardGrid->removeAllWidgets();

	// Set the new size of the grid and the max value of the scrollbar
	unsigned int rowNumber;
	if(widgetsToShow.size() == 0)
		rowNumber = 1;
	else
		rowNumber = ((static_cast<unsigned int>(widgetsToShow.size()) - 1) / GRID_WIDTH) + 1;
	_cardGrid->setSize(tgui::bindWidth(_cardPanel), CardGui::getSize().y * static_cast<float>(rowNumber));
	_scrollbar->setMaximum(rowNumber * static_cast<unsigned int>(CardGui::getSize().y));

	// Fill the grid
	unsigned int i{0U};
	for(auto& widget : widgetsToShow)
	{
		_cardGrid->addWidget(widget, i / GRID_WIDTH, i % GRID_WIDTH);
		++i;
	}
}

void GuiDecksManagementState::selectDeck()
{
	// Update the hint
	_hintLabel->setText(CHOOSE_CARD_FROM_DECK_HINT);
	_hintLabel->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(_hintLabel)) / 2.f, 70);

	Deck& selectedDeck(_decks.at(_decksListBox->getSelectedItemIndex()));
	std::vector<CardWidget::Ptr> widgetsToShow;
	// Browse all cards of the collection
	for(std::size_t i{0}; i < _cardsCollectionWidgets.size(); ++i)
	{
		// If the card is also in the deck to show
		if(std::find(selectedDeck.begin(), selectedDeck.end(), _cardsCollectionIds.at(i)) != selectedDeck.end())
			// Add the corresponding widget to the list of widgets to show
			widgetsToShow.push_back(_cardsCollectionWidgets.at(i));
	}

	fillGrid(widgetsToShow);
}

void GuiDecksManagementState::removeDeck()
{
}

void GuiDecksManagementState::createDeck()
{
}

void GuiDecksManagementState::scrollGrid(int newScrollValue)
{
	_cardGrid->setPosition(_cardGrid->getPosition().x, -static_cast<float>(newScrollValue));
}

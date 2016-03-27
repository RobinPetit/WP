// std-C++ headers
#include <algorithm>
// WizardPoker headers
#include "client/Gui/states/GuiDecksManagementState.hpp"
#include "client/sockets/Client.hpp"
#include "client/Gui/InputBox.hpp"

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
	_chooseCardFromDeck{true},
	_selectedCardFromDeck{}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Decks management", 30U, 30.f);

	// Make the hint label
	_hintLabel->setText(FIRST_HINT);
	_hintLabel->setTextSize(20U);
	_hintLabel->setTextColor(sf::Color::White);
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

void GuiDecksManagementState::onCardClicked(CardId id)
{
	try
	{
		if(_chooseCardFromDeck)
			onCardChosenFromDeck(id);
		else
			onCardChosenFromCollection(id);
		_chooseCardFromDeck = not _chooseCardFromDeck;
	}
	catch(const std::runtime_error& e)
	{
		displayMessage(std::string("Error: ") + e.what());
	}
}

void GuiDecksManagementState::onCardChosenFromDeck(CardId id)
{
	// Update the hint
	_hintLabel->setText(CHOOSE_CARD_FROM_COLLECTION_HINT);
	_hintLabel->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(_hintLabel)) / 2.f, 70);

	// The user cannot change the selected deck when it has to choose a card
	// from the collection
	_decksListBox->disable();

	Deck& selectedDeck(getSelectedDeck());

	// Compute the card collection without the cards of the selected deck
	// For these lines, we just care about IDs
	const std::vector<CardId> cardIdsToShow{getCardsCollectionWithoutDeck(selectedDeck)};
	// Create the corresponding vector of widgets
	DisplayableCardsCollection widgetsToShow;

	// Browse all cards of the collection to show
	for(auto cardToShow : cardIdsToShow)
	{
		// Find cardToShow in the main collection
		auto it(std::find(_cardsCollectionIds.begin(), _cardsCollectionIds.end(), cardToShow));

		// If cardToShow is also in the main collection
		if(it != _cardsCollectionIds.end())
		{
			// Get the index of cardToShow in the main collection
			const std::size_t cardIndex{static_cast<std::size_t>(it - _cardsCollectionIds.begin())};

			// Add the corresponding widget to the list of widgets to show
			widgetsToShow.push_back(_cardsCollectionWidgets.at(cardIndex));
		}
	}

	// Eventually, fill the grid with the computed card collection
	fillGrid(widgetsToShow);

	_selectedCardFromDeck = id;
}

void GuiDecksManagementState::onCardChosenFromCollection(CardId id)
{
	_decksListBox->enable();

	// Show the current deck, this is the same as when we select another deck
	selectDeck();

	// Swap the cards and send the result to the server (through the client)
	Deck& selectedDeck(getSelectedDeck());
	try
	{
		if(std::count(selectedDeck.begin(), selectedDeck.end(), id) >= 2)
			throw std::logic_error(std::to_string(id) + " is already two times in your deck, this is the maximum allowed.");

		const std::size_t replacedIndex{selectedDeck.getIndex(_selectedCardFromDeck)};
		selectedDeck.changeCard(replacedIndex, id);
		_context.client->handleDeckEditing(selectedDeck);
	}
	catch(const std::out_of_range& e)
	{
		displayMessage(std::string("Invalid selected card: ") + e.what());
	}
	catch(const std::runtime_error& e)
	{
		displayMessage(std::string("Error: ") + e.what());
	}
}

void GuiDecksManagementState::fillGrid(const DisplayableCardsCollection& widgetsToShow)
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
	try
	{
		// Update the hint
		_hintLabel->setText(CHOOSE_CARD_FROM_DECK_HINT);
		_hintLabel->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(_hintLabel)) / 2.f, 70);

		Deck& selectedDeck(getSelectedDeck());
		// We have to use a std::vector because we need to erase elements in the loop below
		std::vector<CardId> CardIdsToShow;
		std::copy(selectedDeck.begin(), selectedDeck.end(), std::back_inserter(CardIdsToShow));
		DisplayableCardsCollection widgetsToShow;
		// Browse all cards of the collection
		for(std::size_t i{0}; i < _cardsCollectionWidgets.size(); ++i)
		{
			auto it(std::find(CardIdsToShow.begin(), CardIdsToShow.end(), _cardsCollectionIds.at(i)));
			// If the card is also in the deck to show
			if(it != CardIdsToShow.end())
			{
				// Add the corresponding widget to the list of widgets to show
				widgetsToShow.push_back(_cardsCollectionWidgets.at(i));
				CardIdsToShow.erase(it);
			}

		}

		fillGrid(widgetsToShow);
	}
	catch(const std::runtime_error& e)
	{
		displayMessage(std::string("Error: ") + e.what());
	}
}

void GuiDecksManagementState::removeDeck()
{
	try
	{
		Deck& selectedDeck(getSelectedDeck());
		_context.client->handleDeckDeletion(selectedDeck.getName());
		_decksListBox->removeItem(selectedDeck.getName());
		_decks.erase(std::find_if(_decks.begin(), _decks.end(), [&selectedDeck](const Deck& deck)
		{
			return selectedDeck.getName() == deck.getName();
		}));
	}
	catch(const std::runtime_error& e)
	{
		displayMessage(std::string("Error: ") + e.what());
	}
}

void GuiDecksManagementState::createDeck()
{
	static const std::string messageBoxText{"Enter the name of your\nnew deck:"};
	InputBox::Ptr window{std::make_shared<InputBox>()};

	window->setTitle("Creating a deck");
	window->setLabelText(messageBoxText);
	window->getRenderer()->setTitleBarColor({127, 127, 127});

	auto childWidth{tgui::bindWidth(*_context.gui) * 0.8f};
	auto childHeight{tgui::bindHeight(*_context.gui) * 0.8f};

	window->setSize(childWidth, childHeight);
	window->setGridPosition(childWidth*0.3f, childHeight*0.25f);
	window->setGridSize(childWidth*0.4f, childHeight*0.5f);
	window->setCallback([this](const std::string& deckName)
	{
		try
		{
			_decks.emplace_back(deckName);
			_context.client->handleDeckCreation(_decks.back());
			_decksListBox->addItem(deckName);
		}
		catch(std::runtime_error& e)
		{
			displayMessage(e.what());
		}
	});
	_context.gui->add(window);
}

Deck& GuiDecksManagementState::getSelectedDeck()
{
	auto it(std::find_if(_decks.begin(), _decks.end(), [this](const Deck& deck)
	{
		return deck.getName() == _decksListBox->getSelectedItem();
	}));
	if(it == _decks.end())
		throw std::out_of_range("invalid selected deck.");
	return *it;
}

void GuiDecksManagementState::scrollGrid(int newScrollValue)
{
	_cardGrid->setPosition(_cardGrid->getPosition().x, -static_cast<float>(newScrollValue));
}

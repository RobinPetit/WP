// WizardPoker headers
#include "client/Gui/states/GuiCardsCollectionState.hpp"
#include "client/Gui/CreatureGui.hpp"
#include "client/sockets/Client.hpp"

/*GuiCardsCollectionState::GuiCardsCollectionState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractCardsCollectionState(context),
	_buttons
	{
		{&GuiCardsCollectionState::backMainMenu, "Back to main menu"}
	},
	_cardGrid{std::make_shared<tgui::Grid>()},
	_gridPanel{std::make_shared<tgui::Panel>()},
	_scrollbar{std::make_shared<tgui::Scrollbar>()},
	_cards{}
{
	const unsigned int gridHeight{static_cast<unsigned int>(_cardsCollection.getSize()) / GRID_WIDTH};
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Cards collection", 30U, 20.f);

	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_context.gui->getContainer()));
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight - 30.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, 20.f);

	// Make the panel in the back of the grid
	_gridPanel->setPosition(windowWidth/20.f, 30.f);
	_gridPanel->setSize(windowWidth * 18.f/20.f, windowHeight - 70.f);
	_gridPanel->setBackgroundColor(sf::Color::Transparent);
	_context.gui->add(_gridPanel);

	// Make the scrollbar
	_scrollbar->setPosition(tgui::bindRight(_gridPanel), tgui::bindTop(_gridPanel));
	_scrollbar->setSize((windowWidth - tgui::bindRight(_gridPanel)) / 2.f, tgui::bindHeight(_gridPanel));
	_scrollbar->setLowValue(0);
	_scrollbar->setMaximum(gridHeight * static_cast<unsigned int>(CardGui::getSize().x) / 100U);
	_context.gui->add(_scrollbar);

	// Make the grid
	_cardGrid->setPosition(0.f, 0.f);
	_cardGrid->setSize(tgui::bindWidth(_gridPanel), tgui::bindHeight(_gridPanel));
	_gridPanel->add(_cardGrid);

	unsigned int i{0};
	for(auto& card : _cardsCollection)
	{
		_cards.push_back(std::make_shared<CardWidget>(_context.client->getCardData(card)));
		_cardGrid->addWidget(_cards.back(), i / GRID_WIDTH, i % GRID_WIDTH);
		if(++i > GRID_WIDTH * 3)
			break;
	}
	registerRootWidgets({_buttons[0].button, _gridPanel, _scrollbar});

}
*/

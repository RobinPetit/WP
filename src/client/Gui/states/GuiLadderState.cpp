// WizardPoker headers
#include "client/Gui/states/GuiLadderState.hpp"

GuiLadderState::GuiLadderState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractLadderState(context),
	_buttons
	{
		{&GuiLadderState::backMainMenu, "Back to main menu"}
	},
	_ladderHeader(),
	_panel{std::make_shared<tgui::Panel>()},
	_scrollbar{std::make_shared<tgui::Scrollbar>()},
	_ladderLayout{std::make_shared<tgui::VerticalLayout>()}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));
	float scrollBarWidth = 30;
	float lineHeight = 25;

	// Make the title
	makeTitle("Ladder", 30U, 30.f);

	// Make the button(s)
	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_context.gui->getContainer()));
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight - 30.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, 20.f);

	// Make the panel
	_panel->setPosition(windowWidth/20.f, 70.f);
	_panel->setSize(windowWidth * 18.f/20.f - scrollBarWidth, windowHeight - 110.f);
	_panel->setBackgroundColor(sf::Color::Transparent);
	_context.gui->add(_panel);

	// Make the ladder header
	_ladderHeader.rankLabel->setText("Rank");
	_ladderHeader.playerNameLabel->setText("Player's name");
	_ladderHeader.wonGamesLabel->setText("Won");
	_ladderHeader.playedGamesLabel->setText("Played");
	_ladderHeader.ratioLabel->setText("Ratio");

	_ladderHeader.layout->setPosition(0.f, 0.f);
	_ladderHeader.layout->setSize(tgui::bindWidth(_panel) - 5, lineHeight);
	_panel->add(_ladderHeader.layout);


	// make the layout
	_ladderLayout->setPosition(0.f, lineHeight);
	_ladderLayout->setSize(tgui::bindWidth(_panel) - 5.f, lineHeight * static_cast<float>(_ladder.size()));
	_panel->add(_ladderLayout);

	// Make the scrollbar
	_scrollbar->setPosition(tgui::bindRight(_panel), tgui::bindTop(_panel) + lineHeight);
	_scrollbar->setSize(scrollBarWidth, tgui::bindHeight(_panel) - lineHeight);
	_scrollbar->setLowValue(static_cast<unsigned int>(_panel->getSize().y));
	_scrollbar->setMaximum(static_cast<unsigned int>(lineHeight) * static_cast<unsigned int>(_ladder.size()));
	_scrollbar->setArrowScrollAmount(30);
	_scrollbar->connect("ValueChanged", &GuiLadderState::scrollGrid, this);
	_scrollbar->setAutoHide(false);
	context.gui->add(_scrollbar);

	// add entries to the layout
	for(std::size_t i{0}; i < _ladder.size(); ++i)
	{
		GuiLadderEntry guiLadderEntry;
		if(i%2 == 0)
			guiLadderEntry.setBackgroundColor({200, 200, 200});

		guiLadderEntry.rankLabel->setText(std::to_string(i + 1) + ".");
		guiLadderEntry.playerNameLabel->setText(_ladder[i].name);
		guiLadderEntry.wonGamesLabel->setText(std::to_string(_ladder[i].victories));
		guiLadderEntry.playedGamesLabel->setText(std::to_string(_ladder[i].victories + _ladder[i].defeats));
		guiLadderEntry.ratioLabel->setText(std::to_string(static_cast<float>(_ladder[i].victories) / static_cast<float>(_ladder[i].defeats+_ladder[i].victories)));
		_ladderLayout->add(guiLadderEntry.layout);
	}
	_ladderLayout->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, sf::milliseconds(500));

	registerRootWidgets({_panel, _buttons[0].button, _scrollbar});
}

GuiLadderState::GuiLadderEntry::GuiLadderEntry():
	rankLabel{std::make_shared<tgui::Label>()},
	playerNameLabel{std::make_shared<tgui::Label>()},
	wonGamesLabel{std::make_shared<tgui::Label>()},
	playedGamesLabel{std::make_shared<tgui::Label>()},
	ratioLabel{std::make_shared<tgui::Label>()},
	layout{std::make_shared<tgui::HorizontalLayout>()}
{
	layout->add(rankLabel);
	layout->setFixedSize(rankLabel, 50);
	layout->add(playerNameLabel);

	layout->add(wonGamesLabel);
	layout->setFixedSize(wonGamesLabel, 70);
	layout->add(playedGamesLabel);
	layout->setFixedSize(playedGamesLabel, 70);
	layout->add(ratioLabel);
	layout->setFixedSize(ratioLabel, 70);
}

void GuiLadderState::GuiLadderEntry::setBackgroundColor(const sf::Color& color)
{
	rankLabel->getRenderer()->setBackgroundColor(color);
	playerNameLabel->getRenderer()->setBackgroundColor(color);
	wonGamesLabel->getRenderer()->setBackgroundColor(color);
	playedGamesLabel->getRenderer()->setBackgroundColor(color);
	ratioLabel->getRenderer()->setBackgroundColor(color);
}

void GuiLadderState::scrollGrid(int newScrollValue)
{
	_ladderLayout->setPosition(_ladderLayout->getPosition().x, -static_cast<float>(newScrollValue));
}

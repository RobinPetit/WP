// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Gui/states/GuiLadderState.hpp"

const std::vector<ButtonData<GuiLadderState>> GuiLadderState::_buttons =
{
	{&GuiLadderState::backMainMenu, "Back to main menu"}
};

GuiLadderState::GuiLadderState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractLadderState(context),
	_ladderLayout{std::make_shared<tgui::VerticalLayout>()},
	_titleLabel{std::make_shared<tgui::Label>()}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	// Make the title
	_titleLabel->setText("Ladder");
	_titleLabel->setTextSize(30);
	// center the label on the X axis
	_titleLabel->setPosition(windowWidth/2.f - tgui::bindWidth(_titleLabel)/2.f, 40);
	_context.gui->add(_titleLabel);

	// Make the button(s)
	setupButtons(_buttons, _context.gui);
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight * 8.f/10.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, windowHeight * 1/10.f);

	// Make the ladder header
	_ladderHeader.rankLabel->setText("Rank");
	_ladderHeader.playerNameLabel->setText("Player's name");
	_ladderHeader.wonGamesLabel->setText("Won");
	_ladderHeader.playedGamesLabel->setText("Played");
	_ladderLayout->add(_ladderHeader.layout);

	std::size_t i{0};
	for(auto& guiLadderEntry : _guiLadder)
	{
		// Fill the fields
		guiLadderEntry.rankLabel->setText(std::to_string(i + 1) + ".");
		guiLadderEntry.playerNameLabel->setText(_ladder[i].name);
		guiLadderEntry.wonGamesLabel->setText(std::to_string(_ladder[i].victories));
		guiLadderEntry.playedGamesLabel->setText(std::to_string(_ladder[i].victories + _ladder[i].defeats));
		i++;

		_ladderLayout->add(guiLadderEntry.layout);
	}

	_ladderLayout->setPosition(windowWidth/5.f, windowHeight * 1.2f/10.f);
	_ladderLayout->setSize(windowWidth * 3.f/5.f, windowHeight * 6.5f/10.f);
	_context.gui->add(_ladderLayout);
}

void GuiLadderState::onPush()
{
	_ladderLayout->hide();
	_titleLabel->hide();
}

void GuiLadderState::onPop()
{
	_ladderLayout->show();
	_titleLabel->show();
}

GuiLadderState::GuiLadderEntry::GuiLadderEntry():
	rankLabel{std::make_shared<tgui::Label>()},
	playerNameLabel{std::make_shared<tgui::Label>()},
	wonGamesLabel{std::make_shared<tgui::Label>()},
	playedGamesLabel{std::make_shared<tgui::Label>()},
	layout{std::make_shared<tgui::HorizontalLayout>()}
{
	layout->add(rankLabel);
	layout->setFixedSize(rankLabel, 50);
	layout->add(playerNameLabel);
	layout->add(wonGamesLabel);
	layout->setFixedSize(wonGamesLabel, 50);
	layout->add(playedGamesLabel);
	layout->setFixedSize(playedGamesLabel, 50);
}

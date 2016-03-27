// External headers
#include <TGUI/Animation.hpp>
// WizardPoker headers
#include "client/Gui/states/GuiAchievementState.hpp"

GuiAchievementState::GuiAchievementState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractAchievementState(context),
	_buttons
	{
		{&GuiAchievementState::backMainMenu, "Back to main menu"}
	},
	_panel{std::make_shared<tgui::Panel>()},
	_layout{std::make_shared<tgui::VerticalLayout>()},
	_scrollbar{std::make_shared<tgui::Scrollbar>()},
	_achievementWidgets{}
{
	const unsigned int gridHeight{static_cast<unsigned int>(_achievements.size())};
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	makeTitle("Achievements", 30U, 20.f);

	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_context.gui->getContainer()));
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight - 30.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, 20.f);

	// Make the panel
	_panel->setPosition(windowWidth/20.f, 70.f);
	_panel->setSize(windowWidth * 18.f/20.f, windowHeight - 110.f);
	_panel->setBackgroundColor(sf::Color::Transparent);
	_context.gui->add(_panel);

	// Make the layout
	_layout->setPosition((_panel->getSize().x - AchievementGui::getSize().x)/2, 0.f);
	_layout->setSize(AchievementGui::getSize().x, AchievementGui::getSize().y * static_cast<float>(gridHeight) * 1.1f);
	_panel->add(_layout);

	// Make the scrollbar
	_scrollbar->setPosition(tgui::bindRight(_panel), tgui::bindTop(_panel));
	_scrollbar->setSize((windowWidth - tgui::bindRight(_panel)) / 2.f, tgui::bindHeight(_panel));
	_scrollbar->setOpacity(0.3f);
	_scrollbar->setAutoHide(false);
	_scrollbar->setLowValue(static_cast<unsigned int>(_panel->getSize().y));
	_scrollbar->setMaximum(gridHeight * static_cast<unsigned int>(AchievementGui::getSize().y));
	_scrollbar->setArrowScrollAmount(30);
	_scrollbar->connect("ValueChanged", &GuiAchievementState::scrollGrid, this);
	_context.gui->add(_scrollbar);

	unsigned int i{0};
	for(auto& achievement : _achievements)
	{
		_achievementWidgets.push_back(std::make_shared<AchievementWidget>(achievement));
		_layout->add(_achievementWidgets.back());
		_layout->addSpace(0.1f);
		++i;
	}
	_layout->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, sf::milliseconds(500));
	registerRootWidgets({_buttons[0].button, _panel, _scrollbar});
}

void GuiAchievementState::scrollGrid(int newScrollValue)
{
	_layout->setPosition(_layout->getPosition().x, -static_cast<float>(newScrollValue));
}

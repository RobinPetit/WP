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
	_achievementsLayout{std::make_shared<tgui::VerticalLayout>()}
{
	auto windowWidth(tgui::bindWidth(*_context.gui));
	auto windowHeight(tgui::bindHeight(*_context.gui));

	// Make the title
	makeTitle("Achievements", 30U, 30.f);

	// Make the button(s)
	setupButtons(_buttons, std::static_pointer_cast<tgui::Container>(_context.gui->getContainer()));
	_buttons[0].button->setPosition(windowWidth/5.f, windowHeight * 8.f/10.f);
	_buttons[0].button->setSize(windowWidth * 3.f/5.f, windowHeight * 1/10.f);

	for(std::size_t i{0}; i < _achievements.size(); ++i)
	{
		GuiAchievementEntry guiAchievementEntry;
		guiAchievementEntry.achievementNameLabel->setText(_achievements.at(i).getPrettyName());
		guiAchievementEntry.achievementDescriptionLabel->setText(_achievements.at(i).getDescription());

		_achievementsLayout->add(guiAchievementEntry.layout);
	}

	_achievementsLayout->setPosition(windowWidth/5.f, windowHeight * 1.2f/10.f);
	_achievementsLayout->setSize(windowWidth * 3.5f/5.f, windowHeight * 6.5f/10.f);
	_context.gui->add(_achievementsLayout);

	registerRootWidgets({_achievementsLayout, _buttons[0].button});
}

GuiAchievementState::GuiAchievementEntry::GuiAchievementEntry():
	achievementNameLabel{std::make_shared<tgui::Label>()},
	achievementDescriptionLabel{std::make_shared<tgui::Label>()},
	layout{std::make_shared<tgui::VerticalLayout>()}
{
	layout->add(achievementNameLabel);
	layout->add(achievementDescriptionLabel);
}

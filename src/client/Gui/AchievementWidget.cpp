// WizardPoker headers
#include "client/Gui/AchievementWidget.hpp"
#include "client/Gui/AchievementGui.hpp"


AchievementWidget::AchievementWidget()
{
}

AchievementWidget::AchievementWidget(const ClientAchievement& achievement):
	_achievement(new AchievementGui(achievement.getPrettyName(), achievement.getDescription(), achievement.isUnlocked()))
{
}

void AchievementWidget::setPosition(const tgui::Layout2d& position)
{
	Widget::setPosition(position);
	if(_achievement)
		_achievement->setPosition(position.x.getValue(), position.y.getValue());
}

void AchievementWidget::setSize(const tgui::Layout2d& size)
{
	Widget::setSize(size);
	if(_achievement)
		_achievement->setScale(size.x.getValue()/_achievement->getSize().x, size.y.getValue()/_achievement->getSize().y);
}

void AchievementWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if(_achievement)
	{
		target.draw(*_achievement, states);
	}
}

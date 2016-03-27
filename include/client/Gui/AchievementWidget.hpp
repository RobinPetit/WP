#ifndef _ACHIEVEMENT_WIDGET_CLIENT_HPP
#define _ACHIEVEMENT_WIDGET_CLIENT_HPP

// External headers
#include <TGUI/Widgets/ClickableWidget.hpp>
// WizardPoker header
#include "client/ClientAchievement.hpp"
#include "client/Gui/AchievementGui.hpp"

/// Wrapper around an AchievementGui to be able to add them easily in TGUI containers.
/// The implementation is inspired from CardWidget
class AchievementWidget : public tgui::ClickableWidget
{
	public:
		typedef std::shared_ptr<AchievementWidget> Ptr;
		typedef std::shared_ptr<const AchievementWidget> ConstPtr;

		AchievementWidget();

		AchievementWidget(const ClientAchievement& achievement);

		virtual void setPosition(const tgui::Layout2d& position);
		using tgui::Transformable::setPosition;

		virtual void setSize(const tgui::Layout2d& size);
		using tgui::Transformable::setSize;

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	private:
		std::unique_ptr<AchievementGui> _achievement;
};

#endif  // _ACHIEVEMENT_WIDGET_CLIENT_HPP

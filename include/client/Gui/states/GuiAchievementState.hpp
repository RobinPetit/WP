#ifndef _GUI_ACHIEVEMENT_STATE_CLIENT_HPP
#define _GUI_ACHIEVEMENT_STATE_CLIENT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoker headers
#include "common/Database.hpp"  // Achievements
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractAchievementState.hpp"
#include "client/Gui/AchievementWidget.hpp"


/// Gui representation of the achievements menu state
class GuiAchievementState : public GuiAbstractState, public AbstractAchievementState
{
	public:
		/// Constructor.
		GuiAchievementState(Context& context);

	private:
		/// The type is unsigned rather than size_t because this is the type used by TGUI.
		const std::vector<ButtonData<GuiAchievementState>> _buttons;
		tgui::Panel::Ptr _panel;
		tgui::VerticalLayout::Ptr _layout;
		tgui::Scrollbar::Ptr _scrollbar;
		std::vector<AchievementWidget::Ptr> _achievementWidgets;

		void scrollGrid(int newScrollValue);
};

#endif  // _GUI_ACHIEVEMENT_STATE_CLIENT_HPP

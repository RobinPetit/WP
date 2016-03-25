#ifndef _GUI_ACHIEVEMENT_STATE_CLIENT_HPP
#define _GUI_ACHIEVEMENT_STATE_CLIENT_HPP

// External headers
#include <TGUI/HorizontalLayout.hpp>
#include <TGUI/VerticalLayout.hpp>

// WizardPoker headers
#include "common/Database.hpp"  // Achievements
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractAchievementState.hpp"


/// Gui representation of the achievements menu state
class GuiAchievementState : public GuiAbstractState, public AbstractAchievementState
{
	public:
		/// Constructor.
		GuiAchievementState(Context& context);

	private:
		const std::vector<ButtonData<GuiAchievementState>> _buttons;

		/// A structure that contains all widgets needed for displaying an achievement in GUI
		struct GuiAchievementEntry
		{
			/// Constructor.
			GuiAchievementEntry();

			/// The name of the achievement.
			tgui::Label::Ptr achievementNameLabel;

			/// The description of the achievement.
			tgui::Label::Ptr achievementDescriptionLabel;

			/// The layoud to tie the labels together.
			tgui::VerticalLayout::Ptr layout;
		};

		/// The list of all achievements that will be displayed.
		std::vector<GuiAchievementEntry> _guiAchievements;

		tgui::VerticalLayout::Ptr _achievementsLayout;
};

#endif  // _GUI_ACHIEVEMENT_STATE_CLIENT_HPP

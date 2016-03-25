#ifndef _GUI_GAME_HPP_
#define _GUI_GAME_HPP_

// WizardPoker headers
#include "client/AbstractGame.hpp"
// External headers
#include <TGUI/Gui.hpp>

class GuiGame : public AbstractGame
{
public:
	/// Constructor
	GuiGame(Client& client, tgui::Gui *gui);

private:
	////////// Attributes

	tgui::Gui *_gui;

	////////// Methods

	void displayGame() override;

	void displayMessage(const std::string& message) override;

	int askSelfHandIndex() override;
	int askSelfBoardIndex() override;
	int askSelfGraveyardIndex() override;
	int askOppoBoardIndex() override;
	int askOppoHandIndex() override;

	bool wantToAttackOpponent() override;

	void chooseDeck() override;

	void receiveCard(cardId id) override;
};

#endif  // _GUI_GAME_HPP_

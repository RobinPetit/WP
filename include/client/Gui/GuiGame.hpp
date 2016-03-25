#ifndef _GUI_GAME_HPP_
#define _GUI_GAME_HPP_

// WizardPoker headers
#include "client/AbstractGame.hpp"
#include "common/Deck.hpp"
#include "client/Context.hpp"
// External headers
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/HorizontalLayout.hpp>
// std-C++ headers
#include <utility>

class GuiGame : public AbstractGame
{
public:
	/// Constructor
	GuiGame(Context& context);

	///
	std::pair<tgui::Layout, tgui::Layout> getSize() const;

private:
	////////// Attributes

	Context& _context;

	class DeckWidget
	{
	public:
		/// A function to be called when a button is pressed
		typedef void (GuiGame::*Callback)(const std::string&);

		DeckWidget(const Deck& deck, GuiGame& game, DeckWidget::Callback callback);

		tgui::HorizontalLayout::Ptr getLayout();

	private:
		tgui::Label::Ptr _deckName;
		tgui::Button::Ptr _deckButton;
		tgui::HorizontalLayout::Ptr _layout;
	};

	bool _decksChosen;

	tgui::Layout _width;
	tgui::Layout _height;

	//////////////////// Methods

	////////// overriden methods

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

	////////// private methods

	void sendDeck(const std::string& deckName);
};

#endif  // _GUI_GAME_HPP_

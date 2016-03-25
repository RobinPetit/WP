#ifndef _GUI_GAME_HPP_
#define _GUI_GAME_HPP_

// WizardPoker headers
#include "client/AbstractGame.hpp"
#include "client/Context.hpp"
#include "client/Gui/CardWidget.hpp"
#include "common/Deck.hpp"
// External headers
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/HorizontalLayout.hpp>
// std-C++ headers
#include <vector>

class GuiGame : public AbstractGame
{
public:
	/// Constructor
	GuiGame(Context& context);

	void startTurn() override;

	~GuiGame();

private:
	////////// Attributes

	/// The context to access the client, the gui, the window, etc.
	Context& _context;

	/// The listbox used to display all of the decks when need to choose one
	tgui::ListBox::Ptr _decksListBox;

	/// boolean telling whether or not the deck has already been chosen
	bool _decksChosen;

	// Dimensions of the window

	/// Width of the window
	tgui::Layout _width;
	/// Height of the window
	tgui::Layout _height;

	/// The layout for the player's hand cards
	tgui::HorizontalLayout::Ptr _selfHandLayout;

	//////////////////// Methods

	////////// overriden methods

	/// Displays the whole board: cards, graveyard, health points, etc.
	void displayGame() override;

	/// Display a verbose \a message to the user
	/// \param message The message to show to user
	void displayMessage(const std::string& message) override;

	int askSelfHandIndex() override;
	int askSelfBoardIndex() override;
	int askSelfGraveyardIndex() override;
	int askOppoBoardIndex() override;
	int askOppoHandIndex() override;

	bool wantToAttackOpponent() override;

	/// Displays the decks and ask to user to select the one to play with
	void chooseDeck() override;

	/// Signal the user he received a card
	/// \param id The Id of the card the user received
	void receiveCard(cardId id) override;

	void displayAchievements(ClientAchievementList& newAchievements) override;

	////////// private methods

	/// Sends the chosen deck to the server
	void sendDeck(const std::string& deckName) override;
};

#endif  // _GUI_GAME_HPP_

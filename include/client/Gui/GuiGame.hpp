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
#include <TGUI/Widgets/Grid.hpp>
#include <TGUI/Widgets/Panel.hpp>
// std-C++ headers
#include <vector>

/// Class representing all the interface for the game in graphical mode
///
/// \note The game is multithreaded (a thread listening for server data and
/// another one for the user interactions). Thus, they both could theoretically
/// access the screen simultaneously by calling displayGame for instance.
/// This would cause having the first thread to clear all of the widgets
/// and then having the second one accessing one of the widgets which would
/// cause a SEGFAULT.
///
/// So for a safe behaviours, there is a mutex to lock before to access
/// the screen (and especially changing the widgets). It is `_accessScreen`
/// to use this way:
///
/// \code
/// void GuiGame::displayFunction()
/// {
///     // unique_lock releases the lock automatically when being destroyed
///     std::unique_lock<std::mutex> _lock{_accessScreen};
///     // ...code...
/// }
/// \endcode
class GuiGame : public AbstractGame
{
public:
	/// Constructor
	GuiGame(Context& context);

	/// Called when your turn starts
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

	bool _display;

	// Dimensions of the window

	/// Width of the window
	tgui::Layout _width;
	/// Height of the window
	tgui::Layout _height;

	/// The layout for the player's hand cards
	tgui::Panel::Ptr _selfHandPanel;

	/// The button to voluntarily end your turn
	tgui::Button::Ptr _endTurnButton;

	/// Length (on X axis) of the hand cards
	const tgui::Layout _cardsLayoutWidth;

	/// A card to display (in big) when passing the mouse over a hand card
	CardWidget::Ptr _readableCard;

	/// A boolean telling whether or not the big card is displayed at a very moment
	bool _isBigCardOnBoard;

	tgui::Panel::Ptr _opponentBoardPanel;

	tgui::Panel::Ptr _selfBoardPanel;

	std::vector<CardWidget::Ptr> _selfHand;

	std::vector<CardWidget::Ptr> _selfBoard;

	std::vector<CardWidget::Ptr> _opponentBoard;

	//////////////////// Methods

	////////// overriden methods

	/// Displays the whole board: cards, graveyard, health points, etc.
	/// Locks the screen.
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

	/// Waits for \a booleanFunction to return true and treats events in
	/// the mean time
	/// \param booleanFunction A function returning true when waiting should stop
	/// and false as long as looping is required
	void waitUntil(std::function<bool()> booleanFunction) override;

	////////// private methods

	/// Sends the chosen deck to the server
	/// \param deckName The name of the deck that's been chosen
	void sendDeck(const std::string& deckName) override;

	/// Loops on events for as long as the turn is yours
	void handleInputs();

	/// Resets the screen and then redraws the current interface
	void refreshScreen();

	/// Function to display the cards the user has in his hands
	/// Does not lock the screen.
	void displayHandCards();

	/// Function to display the user board
	void displaySelfBoard();

	/// Function to draw the opponent board
	void displayOpponentBoard();

	/// Function to display a board on a panel
	/// \param panel The panel to display the cars on
	/// \param graphicalCards The cards that will be created and displayed
	/// \param creatureDatas The cards used to create the Gui cards
	/// \param reversed A boolean telling if card must be displayed upside down
	void displayPlayerBoard(tgui::Panel::Ptr& panel, std::vector<CardWidget::Ptr>& graphicalCards,
		std::vector<BoardCreatureData>& creatureDatas, bool reversed=false);
};

#endif  // _GUI_GAME_HPP_

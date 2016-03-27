#ifndef _GUI_GAME_HPP_
#define _GUI_GAME_HPP_

// WizardPoker headers
#include "client/AbstractGame.hpp"
#include "client/Context.hpp"
#include "client/Gui/CardWidget.hpp"
#include "common/Deck.hpp"
#include "client/Gui/CreatureGui.hpp"
#include "client/Gui/AchievementWidget.hpp"
// External headers
#include <TGUI/Gui.hpp>
#include <TGUI/VerticalLayout.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/Panel.hpp>
// std-C++ headers
#include <vector>
#include <atomic>
#include <queue>

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
	typedef void (GuiGame::*CardCallback)(int);
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

	/// A layout that contains information labels about self, and the end turn button
	tgui::VerticalLayout::Ptr _selfInfoLayout;

	/// Various information labels
	tgui::Label::Ptr _selfHealthLabel;
	tgui::Label::Ptr _selfEnergyLabel;
	tgui::Label::Ptr _selfDeckSizeLabel;
	tgui::Label::Ptr _selfGraveyardSizeLabel;
	tgui::Label::Ptr _gameInfoLabel;

	/// The button to voluntarily end your turn
	tgui::Button::Ptr _endTurnButton;

	/// The button to directly attack the opponent
	tgui::Button::Ptr _attackOpponentButton;

	/// A layout that contains information labels about the opponent
	tgui::VerticalLayout::Ptr _oppoInfoLayout;

	/// Various information labels
	tgui::Label::Ptr _oppoHealthLabel;
	tgui::Label::Ptr _oppoHandSizeLabel;

	/// Length (on X axis) of the hand cards
	const tgui::Layout _cardsLayoutWidth;

	/// A card to display (in big) when passing the mouse over a hand card
	CardWidget::Ptr _readableCard;

	/// A boolean telling whether or not the big card is displayed at a very moment
	bool _isBigCardOnBoard;

	/// The panel containing the board of the opponent
	tgui::Panel::Ptr _opponentBoardPanel;

	/// The panel containing the board of the user
	tgui::Panel::Ptr _selfBoardPanel;

	/// The cards in the user's hands
	DisplayableCardsCollection _selfHand;

	/// The cards on the user's board
	DisplayableCardsCollection _selfBoard;

	/// The cards on the opponent's board
	DisplayableCardsCollection _opponentBoard;

	// take negative values to select nothing
	int _currentSelfSelection;
	int _currentOpponentSelection;

	/// A value meaning that no card is currently selected
	static constexpr int NoSelection{-1};

	bool _activeCallbacks;

	std::atomic_bool _displayRequest;

	std::thread::id _ownerId;

	std::mutex _accessMessages;

	std::queue<std::string> _pendingMessages;

	//////////////////// Methods

	////////// overriden methods

	/// Displays the whole board: cards, graveyard, health points, etc.
	/// Locks the screen.
	void displayGame() override;

	/// Display a verbose \a message to the user
	/// \param message The message to show to user
	void displayMessage(const std::string& message) override;

	void displayGameInfo(const std::string& message);

	/// Disables the callbacks
	void removeClickableCallbacks();

	void updateDisplay() override;

	void onListeningThreadCreation() override;

	/// Waits for a click on a card in the vector \a cards
	/// \param cards The collection of the clickable cards
	int askIndexFromVector(DisplayableCardsCollection& cards);

	int askSelfHandIndex() override;
	int askSelfBoardIndex() override;
	int askSelfGraveyardIndex() override;
	int askOppoBoardIndex() override;
	int askOppoHandIndex() override;

	/// Displays the decks and ask to user to select the one to play with
	void chooseDeck() override;

	/// Signal the user he received a card
	/// \param id The Id of the card the user received
	void receiveCard(CardId id) override;

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

	/// Fills the screen in white
	void clearScreen();

	/// Sets the correct value on the gui cards according to the
	/// protected inherited attributes
	void updateGuiCardValues();

	/// Interface to update the values on a whole board (player or opponent)
	/// \see updateCard
	/// \param dataBoard A vector of card data
	/// \param guiBoard A vector of displayable cards
	void updateGuiBoard(std::vector<BoardCreatureData>& dataBoard, DisplayableCardsCollection& guiBoard);

	/// Changes the values on a graphical card according to \a data
	/// \param card A displayable card to update
	/// \param data The values to use to replace the card
	void updateCard(CreatureGui *card, const BoardCreatureData& data);

	/// Resets the screen and then redraws the current interface
	void refreshScreen();

	/// Waits until event are available, and process them.
	void processWindowEvents();

	/// Callback called when a card on the user's board is clicked
	/// \param index The index of the card that's been clicked
	void handleSelfBoardClick(int index);

	/// Callback called when a card on the opponent's board is clicked
	/// \param index The index of the card that's been clicked
	void handleOpponentBoardClick(int index);

	/// Reset the board when the user unselected one of his cards
	void unselectSelfCard();

	/// Function to allow "big card" to be displayed when mouse passes over the card
	/// \param card The card to pass on to draw the big card
	/// \param cardData The data to create the big card
	void connectBigCardDisplay(CardWidget::Ptr& card, const CommonCardData *cardData, const BoardCreatureData *data=nullptr);

	/// Creates the new card to be displayed at the center of the screen
	/// \param cardData The data used to create the card
	void createBigCard(const CommonCardData *cardData);

	/// Displays the big card
	/// \see createBigCard
	/// \param cardData The data used to create the card
	void displayBigCard(const CommonCardData *cardData);

	/// Displays the big card and updates it according to \a data
	/// \param cardData The data used to create the prototype of the big card
	/// \param data The data used to update the values on the card
	void displayBigCreature(const CommonCardData *cardData, const BoardCreatureData& data);

	/// Function to display the cards the user has in his hands
	/// Does not lock the screen.
	void displayHandCards();

	/// Function to display the user board
	void displaySelfBoard();

	/// Function to draw the opponent board
	void displayOpponentBoard();

	/// Function to draw the labels containing player's informations
	void displayInfo();

	/// Function to display a board on a panel
	/// \param panel The panel to display the cards on
	/// \param graphicalCards The cards that will be created and displayed
	/// \param creatureDatas The cards used to create the Gui cards
	/// \param callback A method pointer to be called when card is clicked
	/// (nullptr for no callback)
	/// \param reversed A boolean telling if card must be displayed upside down
	/// \param displayableWhenMouseOver A boolean telling if the "big card"
	/// should be displayed when the mouse passes over the card
	void displayPlayerBoard(tgui::Panel::Ptr& panel, DisplayableCardsCollection& graphicalCards,
		std::vector<BoardCreatureData>& creatureDatas, CardCallback callback=nullptr,
		bool reversed=false, bool displayableWhenMouseOver=true);
};

#endif  // _GUI_GAME_HPP_

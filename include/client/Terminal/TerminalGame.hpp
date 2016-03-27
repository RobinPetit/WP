#ifndef TERMINAL_GAME_HPP
#define TERMINAL_GAME_HPP

// WizardPoker headers
#include "client/AbstractGame.hpp"

class TerminalGame : public AbstractGame
{
public:
	TerminalGame(Client& client);

	void startTurn() override;

	~TerminalGame();

private:
	//////////// static members

	static const std::vector<std::pair<const std::string, void (TerminalGame::*)()>> _actions;

	////////// private methods

	void displayOptions();
	void displayGame() override;
	void displayCardVector(const std::vector<CardData>& cardVector, bool displayDescription=false);
	void displayBoardCreatureVector(const std::vector<BoardCreatureData>& cardVector, bool displayDescription=false);

	void displayMessage(const std::string& message) override;
	void displayAchievements(ClientAchievementList& newAchievements) override;

	// Requests user for additional input
	int askIndex(std::size_t upperBound, const std::string& inputMessage);

	int askSelfHandIndex() override;
	int askSelfBoardIndex() override;
	int askSelfGraveyardIndex() override;
	int askOppoBoardIndex() override;
	int askOppoHandIndex() override;

	bool wantToAttackOpponent() override;

	void chooseDeck() override;

	void receiveCard(CardId id) override;

	// overload
	void useCard();
	void attackWithCreature();
};

#endif  // TERMINAL_GAME_HPP

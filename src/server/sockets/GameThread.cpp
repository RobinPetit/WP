// WizardPoker headers
#include "server/GameThread.hpp"
#include "common/sockets/TransferType.hpp"
#include "server/Creature.hpp"
// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/System/Sleep.hpp>
// std-C++ headers
#include <iostream>
#include <chrono>

constexpr std::chrono::seconds GameThread::_turnTime;

GameThread::GameThread(ServerDatabase& database, userId player1ID, userId player2ID):
	std::thread(),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(false),
	_player1(_player1ID, _socketPlayer1, _specialOutputSocketPlayer1),
	_player2(_player2ID, _socketPlayer2, _specialOutputSocketPlayer2),
	_database(database),
	_turn(0),
	_turnCanEnd(false)
{
	createPlayers();
}

void GameThread::createPlayers()
{
	_activePlayer = &_player1;
	_passivePlayer = &_player2;
	std::default_random_engine engine;
	if(std::bernoulli_distribution(0.5)(engine))
		std::swap(_activePlayer, _passivePlayer);
	_activePlayer->setOpponent(_passivePlayer);
	_passivePlayer->setOpponent(_activePlayer);
}

// \TODO: complete the function as a QuitGame
void GameThread::interruptGame()
{
	_running.store(false);
	//TODO: need identifier for player who quit
	//NETWORK: PLAYER_QUIT_GAME
}

void GameThread::establishSockets(const ClientInformations& player1, const ClientInformations& player2)
{
	setSocket(_socketPlayer1, _specialOutputSocketPlayer1, player1);
	setSocket(_socketPlayer2, _specialOutputSocketPlayer2, player2);
}

void GameThread::setSocket(sf::TcpSocket& socket, sf::TcpSocket& specialSocket, const ClientInformations& player)
{
	sf::TcpSocket tmpSocket;
	tmpSocket.connect(player.socket->getRemoteAddress(), player.listeningPort);
	sf::TcpListener listener;
	listener.listen(sf::Socket::AnyPort);
	sf::Packet packet;
	packet << TransferType::NEW_GAME_SERVER_CONNECTION << listener.getLocalPort();
	tmpSocket.send(packet);
	// Here, the client connects itself
	if(listener.accept(socket) != sf::Socket::Done)
		std::cerr << "Error while creating game thread socket\n";
	// and the "specialSocket" used to send important data which are
	// not blocking the main client thread (eg: END_OF_TURN, BOARD_UPDATE, etc.)
	if(listener.accept(specialSocket) != sf::Socket::Done)
		std::cerr << "Error while creating game thread special socket\n";
}

void GameThread::receiveDecks()
{
	std::cout << "waiting for decks\n";
	sf::Packet deckPacket;
	TransferType type;

	_socketPlayer1.receive(deckPacket);
	deckPacket >> type;
	if(type != TransferType::GAME_PLAYER_GIVE_DECK_NAMES)
		throw std::runtime_error("Unable to get player 1's deck");
	deckPacket >> _player1DeckName;

	_socketPlayer2.receive(deckPacket);
	deckPacket >> type;
	if(type != TransferType::GAME_PLAYER_GIVE_DECK_NAMES)
		throw std::runtime_error("Unable to get player 2's deck");
	deckPacket >> _player2DeckName;

	_player1.setDeck(_database.getDeckByName(_player1ID, _player1DeckName));
	_player2.setDeck(_database.getDeckByName(_player2ID, _player2DeckName));
}

void GameThread::startGame(const ClientInformations& player1, const ClientInformations& player2)
{
	establishSockets(player1, player2);
	sf::Packet packet;
	// get players decks
	receiveDecks();

	// send the game is starting
	packet << TransferType::GAME_STARTING << TransferType::GAME_PLAYER_ENTER_TURN;
	_activePlayer->getSocket().send(packet);
	packet.clear();
	packet << TransferType::GAME_STARTING << TransferType::GAME_PLAYER_LEAVE_TURN;
	_passivePlayer->getSocket().send(packet);
	_timerThread = std::thread(&GameThread::makeTimer, this);

	runGame();

	packet.clear();
	packet << TransferType::GAME_OVER;
	// same message is sent to both players: no need to find by ID
	_specialOutputSocketPlayer1.send(packet);
	_specialOutputSocketPlayer2.send(packet);
}

void GameThread::runGame()
{
	// prepare data listening
	sf::SocketSelector selector;
	selector.add(_socketPlayer1);
	selector.add(_socketPlayer2);
	sf::Packet playerActionPacket;
	while(_running.load())
	{
		if(!selector.wait(sf::milliseconds(50)))
			continue;
		// Get actions from playing client
		sf::TcpSocket& modifiedSocket{selector.isReady(_socketPlayer1) ? _socketPlayer1 : _socketPlayer2};
		modifiedSocket.receive(playerActionPacket);

		// Check that only actions from the active player are handled
		sf::TcpSocket& activePlayerSocket{_activePlayer == &_player1 ? _socketPlayer1 : _socketPlayer2};

		TransferType type;
		playerActionPacket >> type;
		// The GAME_GUIT_GAME action can be send also by passive player, so we
		// must handle it separately
		if(type == GAME_QUIT_GAME)
		{
			// quit game
		}
		else
		{
			switch(type)
			{
				case TransferType::GAME_PLAYER_LEAVE_TURN:
					_turnSwap.store(true);
					break;

				case TransferType::GAME_USE_CARD:
				{
					sf::Int32 cardIndex;
					playerActionPacket >> cardIndex;
					useCard(static_cast<int>(cardIndex));
					break;
				}

				case TransferType::GAME_ATTACK_WITH_CREATURE:
				{
					sf::Int32 attackerIndex, victimIndex;
					playerActionPacket >> attackerIndex >> victimIndex;
					attackWithCreature(static_cast<int>(attackerIndex), static_cast<int>(victimIndex));
					break;
				}

				default:
					std::cout << "GameThread::runGame error: wrong packet header, "
					"expected user in-game action header.\n";
					break;
			}
		}
		playerActionPacket.clear();
	}
}

void GameThread::endTurn()
{
	_turn++;  // turn counter (for both players)
	_activePlayer->leaveTurn();
	std::swap(_activePlayer, _passivePlayer);  // swap active and inactive
	_activePlayer->enterTurn(_turn/2 +1);  // ALWAYS call active player
}

// Function only called by a new thread
void GameThread::makeTimer()
{
	static const std::chrono::milliseconds sleepingTime{50};
	auto startOfTurnTime{std::chrono::high_resolution_clock::now()};
	while(_running.load())
	{
		std::this_thread::sleep_for(sleepingTime);
		// if the current player didn't finished his turn and he still has got time to play, wait
		if(!_turnSwap.load() && std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startOfTurnTime) < _turnTime)
			continue;
		// send to both players their turn swapped
		sf::Packet endOfTurn;
		endOfTurn << TransferType::GAME_PLAYER_LEAVE_TURN;
		_activePlayer->getSpecialSocket().send(endOfTurn);

		sf::Packet startOfTurn;
		startOfTurn << TransferType::GAME_PLAYER_ENTER_TURN;
		_passivePlayer->getSpecialSocket().send(startOfTurn);

		endTurn();
		startOfTurnTime = std::chrono::high_resolution_clock::now();
		_turnSwap.store(false);
	}
}

GameThread::~GameThread()
{
	interruptGame();
	_timerThread.join();
}

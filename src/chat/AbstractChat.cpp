// WizardPoker headers
#include "chat/AbstractChat.hpp"
#include "common/UnableToConnectException.hpp"
#include "common/sockets/TransferType.hpp"
// std-C++ headers
#include <sstream>

AbstractChat::AbstractChat(const char * const argv[6]):
	_role{argv[1]},
	_address{argv[2]},
	// argv[3] is the port which is initialized in the body of the function
	_selfName{argv[4]},
	_friendName{argv[5]},
	_running{false}
{
	std::stringstream(argv[3]) >> _gameServerPort;
}

void AbstractChat::connect()
{
	_listener.listen(sf::Socket::AnyPort);
	if(_role == "caller")
		connectAsCaller();
	else if(_role == "callee")
		connectAsCallee();
	else
		throw std::runtime_error("Unknown role: " + _role);
	onConnection();
}

void AbstractChat::onConnection()
{

}

void AbstractChat::connectAsCaller()
{
	// for now, _out is a connection to the game server
	_out.connect(_address, _gameServerPort);
	// send the caller's informations to the server
	sf::Packet dataPacket;
	dataPacket << TransferType::CHAT_PLAYER_IP << _selfName << _friendName << _listener.getLocalPort();
	_out.send(dataPacket);
	// and expect the callee's informations to be received
	_out.receive(dataPacket);
	// we don't need the server anymore
	_out.disconnect();

	// if header is failure, abort: server cannot give the desired informations
	TransferType responseHeader;
	dataPacket >> responseHeader;
	if(responseHeader == TransferType::FAILURE)
		throw std::runtime_error("Unable to start a discussion");

	// if the header is not FAILURE, then get the friend's address
	sf::Uint32 friendAddress;
	dataPacket >> friendAddress;

	// have the friend connecting to the input socket
	_listener.accept(_in);
	// the friend gives its listening port at first connection
	_in.receive(dataPacket);
	sf::Uint16 friendListeningPort;
	dataPacket >> friendListeningPort;

	// Once every information about the friend is got, let's connect to friend
	if(_out.connect(_in.getRemoteAddress(), friendListeningPort) != sf::Socket::Done)
		throw UnableToConnectException("Unable to connect to friend");
}

void AbstractChat::connectAsCallee()
{
	// connect to friend
	_out.connect(_address, _gameServerPort);
	// send to friend the listening port
	sf::Packet dataPacket;
	dataPacket << _listener.getLocalPort();
	_out.send(dataPacket);
	// have the friend connecting to the input socket
	_listener.accept(_in);
}

void AbstractChat::start()
{
	connect();
	_inputThread = std::thread(&AbstractChat::input, this);
	_friendPresence.store(true);
	_running.store(true);
	output();
}

void AbstractChat::treatMessage(const std::string& message)
{
	// special inputs start with ':'
	if(message[0] != ':')
	{
		if(_friendPresence.load())
		{
			sf::Packet messagePacket;
			messagePacket << TransferType::CHAT_MESSAGE << message;
			_out.send(messagePacket);
		}
		display(_selfName, message);
	}
	else if(message == ":quit")
		endDiscussion();
}

void AbstractChat::endDiscussion()
{
	_running.store(false);
	if(_friendPresence.load())
	{
		sf::Packet goodbyePacket;
		goodbyePacket << TransferType::CHAT_QUIT;
		_out.send(goodbyePacket);
	}
}

void AbstractChat::input()
{
	_in.setBlocking(false);
	sf::Packet inputPacket;
	std::string message;

	while(_running.load())
	{
		sf::Socket::Status status{_in.receive(inputPacket)};
		if(status == sf::Socket::NotReady)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		else if(status == sf::Socket::Done)
		{
			TransferType responseHeader;
			inputPacket >> responseHeader;
			switch(responseHeader)
			{
			case TransferType::CHAT_MESSAGE:
				inputPacket >> message;
				display(_friendName, message);
				break;

			case TransferType::CHAT_QUIT:
				display(_friendName, "left the discussion");
				_friendPresence.store(false);
				break;

			default:
				throw std::runtime_error("AbstractChat::input: Wrong header");
			}
		}
		else
			throw std::runtime_error("AbstractChat::input: Error in received status");
	}
}

AbstractChat::~AbstractChat()
{
	_running.store(false);
	if(_inputThread.joinable())
		_inputThread.join();
}

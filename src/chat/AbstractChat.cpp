// WizardPoker headers
#include "chat/AbstractChat.hpp"
#include "common/UnableToConnectException.hpp"
#include "common/sockets/TransferType.hpp"
// std-C++ headers
#include <sstream>

const std::wstring AbstractChat::_quitString = L":quit";

AbstractChat::AbstractChat(const char * const argv[6]):
	_selfName{argv[4]},
	_friendName{argv[5]},
	_running{false},
	_address{argv[2]},
	_role{argv[1]},
	_listening{false}
{
	std::stringstream(argv[3]) >> _remotePort;
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
	// Function has empty body because default behaviour is to do nothing
	// when both players are conected. But having a pure virtual method
	// would force child classes to define onConnection even though they
	// don't want to do anything. Choice is then to have an empty-function
	// here and to not bother developpers of child classes if they don't
	// want to
}

void AbstractChat::connectAsCaller()
{
	// for now, _out is a connection to the game server
	_out.connect(_address, _remotePort);
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
	_out.connect(_address, _remotePort);
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
	// create awaiting thread
	_inputThread = std::thread(&AbstractChat::input, this);
	_friendPresence.store(true);
	_running.store(true);
	output();
}

void AbstractChat::treatMessage(const std::wstring& message)
{
	// special inputs start with ':'
	if(message[0] != ':')
	{
		// only send message if friend is still connected
		if(_friendPresence.load())
		{
			sf::Packet messagePacket;
			messagePacket << TransferType::CHAT_MESSAGE << message;
			_out.send(messagePacket);
		}
		display(_selfName, message);
	}
	else if(message == _quitString)
		endDiscussion();
}

void AbstractChat::endDiscussion()
{
	_running.store(false);
	// if friend already left, send nothing
	if(not _friendPresence.load())
		return;
	// otherwise send a "goodbye message"
	sf::Packet goodbyePacket;
	goodbyePacket << TransferType::CHAT_QUIT;
	_out.send(goodbyePacket);
}

void AbstractChat::input()
{
	_listening.store(true);
	_in.setBlocking(false);
	sf::Packet inputPacket;
	std::wstring message;

	while(_listening.load())
	{
		sf::Socket::Status status{_in.receive(inputPacket)};
		// If no packet wassent by friend, wait 1/10 s
		if(status == sf::Socket::NotReady)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		// If something arrived,
		else if(status == sf::Socket::Done)
		{
			// it should be in {CHAT_MESSAGE, CHAT_QUIT}
			TransferType responseHeader;
			inputPacket >> responseHeader;
			switch(responseHeader)
			{
			case TransferType::CHAT_MESSAGE:
				inputPacket >> message;
				display(_friendName, message);
				break;

			case TransferType::CHAT_QUIT:
				friendQuit();
				break;

			default:
				throw std::runtime_error("AbstractChat::input: Wrong header");
			}
		}
		else if(status == sf::Socket::Disconnected)
			friendQuit();
		else  // status == sf::Socket::Error
			throw std::runtime_error("AbstractChat::input: Error in received status");
	}
}

void AbstractChat::friendQuit()
{
	_listening.store(false);
	display(_friendName, L"left the discussion");
	_friendPresence.store(false);
	_in.disconnect();
	_out.disconnect();
}

AbstractChat::~AbstractChat()
{
	_running.store(false);
	if(_inputThread.joinable())
		_inputThread.join();
}

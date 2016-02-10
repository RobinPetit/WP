#include <client/sockets/Client.hpp>
#include <common/constants.hpp>
#include <common/sockets/TransferType.hpp>
#include <iostream>
#include <thread>
#include <cstdlib>

extern void chatListening(sf::Uint16 *port, std::atomic_bool *loop);

Client::Client(const std::string& name):
	_socket(),
	_currentConversations(),
	_isConnected(false),
	_chatListenerPort(0),
	_threadLoop(0)
{
	// forces the name to not be larger than MAX_NAME_LENGTH
	_name = (name.size() < MAX_NAME_LENGTH) ? name : name.substr(0, MAX_NAME_LENGTH);
}

bool Client::connectToServer(const sf::IpAddress& address, int port)
{
	// if client is already connected to a server, do not try to re-connect it
	if(_isConnected)
		return false;
	initListener();  // creates the new thread which listens for entring chat conenctions
	// if connection does not work, don't go further
	if(_socket.connect(address, port) != sf::Socket::Done)
		return false;
	sf::Packet packet;
	packet << TransferType::GAME_CONNECTION  // precise
	       << _name  // do not forget the '\0' character
	       << static_cast<sf::Uint16>(_chatListenerPort);
	if(_socket.send(packet) != sf::Socket::Done)
		return false;
	_isConnected = true;
	return true;
}

bool Client::startConversation(const std::string& playerName)
{
	// rest assured the client is connected to a server before trying to access it
	if(!_isConnected)
		return false;
	std::string cmd;
	cmd = "gnome-terminal "
	      "-x ./WizardPoker_chat "
	      "\"caller\" "
	      "\"" SERVER_ADDRESS "\" "
	      "\"" + _name + "\" "
	      "\"" + playerName + "\"";
	system(cmd.c_str());
	return true;
}

void Client::initListener()
{
	_threadLoop.store(true);
	_listenerThread = std::thread(chatListening, &_chatListenerPort, &_threadLoop);
}

void Client::quit()
{
	_threadLoop.store(false);
	_listenerThread.join();
}

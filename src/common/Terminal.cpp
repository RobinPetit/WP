// WizardPoker headers
#include "common/Terminal.hpp"
// std-C++ headers
#include <cstdlib>
#include <string>
#include <fstream>
#include <initializer_list>
// SFML headers
#include <SFML/System/Sleep.hpp>

const std::string Terminal::_terminalNames[TERMINALS_LENGTH] =
{
	// Linux
	"gnome-terminal",
	"mate-terminal",
	// Windows
	"cmd",
	"<None>",
};

const std::string Terminal::_tmpTerminalsFileName = "_terminals.tmp";

Terminal::Terminal():
	_idx(NO_KNOWN_TERMINAL)
{
#ifdef __linux__
	std::string terms;
	for(int idx = TERMINAL_LIST_BEGIN; idx < TERMINAL_LIST_END; ++idx)
		terms += _terminalNames[idx] + " ";
	terms += _terminalNames[TERMINAL_LIST_END];
	system(("which " + terms + ">" + _tmpTerminalsFileName).c_str());
	sf::sleep(sf::milliseconds(100));  // give time to the `which` program
	std::ifstream result;
	result.open(_tmpTerminalsFileName.c_str());
	std::string firstLine;
	std::getline(result, firstLine);
	if(firstLine != "")
	{
		bool loop(true);
		int idx = TERMINAL_LIST_BEGIN;
		while(loop && idx <= TERMINAL_LIST_END)
		{
			if(firstLine.find(_terminalNames[idx]) != std::string::npos)
			{
				loop = false;
				_idx = idx;
			}
			++idx;
		}
	}
	result.close();
	remove(_tmpTerminalsFileName.c_str());
#else
	_idx = CMD;
#endif
}

const std::string& Terminal::getTerminalName() const
{
	return _terminalNames[_idx];
}

bool Terminal::hasKnownTerminal() const
{
	return _idx != NO_KNOWN_TERMINAL;
}

std::string Terminal::startProgram(const std::string& name, const std::initializer_list<const std::string>& args, bool openInNewTerminal) const
{
	if(openInNewTerminal and not hasKnownTerminal())
		throw std::runtime_error("Unable to start new program: no known terminal on this computer");
	std::string command;
#ifdef __linux__
	command = (openInNewTerminal ? (getTerminalName() + " -x ") : "") + "./" + name;
#else
	command = "start " + name;
#endif
	for(auto& arg: args)
		command += " \"" + arg + "\"";
	return command;
}

void Terminal::clearScreen()
{
#ifdef __linux__
	system("clear");
#else
	system("cls");
#endif
}

std::string Terminal::setBold(const std::string& message)
{
#ifdef __linux__
	return "\033[1m" + message + "\033[0m";
#else
	return message;
#endif
}

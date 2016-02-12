#ifndef _TERMINAL_COMMON_HPP_
#define _TERMINAL_COMMON_HPP_

#include <string>

#ifdef __linux__
# define TERMINAL_LIST_BEGIN GNOME
# define TERMINAL_LIST_END MATE
#else
# define TERMINAL_LIST_BEGIN CMD
# define TERMINAL_LIST_END CMD
#endif

/// Terminal is a class that allow to find the terminal installed on
/// the computer (among some predefined)
class Terminal
{
public:
	/// Constructor
	Terminal();

	/// This method gives the name of the terminal as used for the system function
	/// \return The string that needs to be used to start a new terminal
	const std::string& getTerminalName();

	/// This function must be used in the beggining of the client program
	/// to know whether or not a chat will be used
	/// \return True if the program found a terminal it knows and false otherwise
	bool hasKnownTerminal();
private:
	static const std::string _terminalNames[];
	int _idx;
};

#endif // _TERMINAL_COMMON_HPP_

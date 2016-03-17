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
	const std::string& getTerminalName() const;

	/// This function must be used in the beginning of the client program
	/// to know whether or not a chat will be used
	/// \return True if the program found a terminal it knows and false otherwise
	bool hasKnownTerminal() const;

	/// This function returns the right command to start a new program
	/// \param name A string containing the name of the program to be started
	/// \param args A list representing the parameters given to the called program
	std::string startProgram(const std::string& name, const std::initializer_list<const std::string>& args) const;

	/// This function clears the content of the terminal
	static void clearScreen();

	static std::string setBold(const std::string& message);
private:
	static const std::string _terminalNames[];
	int _idx;
};

#endif // _TERMINAL_COMMON_HPP_

#ifndef _TERMINAL_CHAT_
#define _TERMINAL_CHAT_

// WizardPoer headers
#include "chat/AbstractChat.hpp"
#include "common/Terminal.hpp"

class TerminalChat : public AbstractChat
{
public:
	/// Constructor
	TerminalChat(const char * const argv[6]);

	void output() override;

	void display(const std::string& name, const std::string& message) override;

	// default constructor is sufficient
	~TerminalChat() = default;

private:
	/// A Terminal handler to set text in bold or to clear screen
	Terminal _terminal;
};

#endif  // _TERMINAL_CHAT_

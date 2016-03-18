#ifndef _GUI_CHAT_CHAT_HPP
#define _GUI_CHAT_CHAT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoer headers
#include "chat/AbstractChat.hpp"
#include "common/Terminal.hpp"

class GuiChat : public AbstractChat
{
public:
	GuiChat(const char * const argv[6]);

	void output() override;

	void display(const std::string& name, const std::string& message) override;

private:
	// Methods
	void handleSendButtonCallback();

	// Attributes
	sf::VideoMode _videoMode;
	std::string _windowName;
	sf::RenderWindow _window;
	tgui::Gui _gui;
	tgui::VerticalLayout::Ptr _mainLayout;
	tgui::ChatBox::Ptr _chatBox;
	tgui::HorizontalLayout::Ptr _messageLayout;
	tgui::EditBox::Ptr _messageEditBox;
	tgui::Button::Ptr _sendButton;
};

#endif  // _GUI_CHAT_CHAT_HPP

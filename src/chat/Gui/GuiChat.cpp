// WizardPoker headers
#include "chat/Gui/GuiChat.hpp"

GuiChat::GuiChat(const char * const argv[6]):
	AbstractChat(argv),
	_videoMode{400, 400},
	_windowName{"Wizard Poker chat: " + _friendName},
	_window{_videoMode, _windowName},
	_gui{_window},
	_mainLayout{std::make_shared<tgui::VerticalLayout>()},
	_chatBox{std::make_shared<tgui::ChatBox>()},
	_messageLayout{std::make_shared<tgui::HorizontalLayout>()},
	_messageEditBox{std::make_shared<tgui::EditBox>()},
	_sendButton{std::make_shared<tgui::Button>()}
{
	auto windowWidth(tgui::bindWidth(_gui));
	auto windowHeight(tgui::bindHeight(_gui));

	_sendButton->setText("Send");
	_sendButton->connect("Pressed", &GuiChat::handleSendButtonCallback, this);
	_messageEditBox->connect("ReturnKeyPressed", &GuiChat::handleSendButtonCallback, this);

	// Add widgets to the message layout
	_messageLayout->add(_messageEditBox);
	_messageLayout->add(_sendButton);
	_messageLayout->setFixedSize(_sendButton, 40);

	// Add widgets to the main layout
	_mainLayout->add(_chatBox);
	_mainLayout->add(_messageLayout);
	_mainLayout->setFixedSize(_messageLayout, 20);

	// Add the main layout to the GUI
	_mainLayout->setSize(windowWidth, windowHeight);
	_mainLayout->setPosition(0, 0);
	_gui.add(_mainLayout);

	// Fix a bug that occurs on Theo's computer, showing a "This window does not
	// respond, do you wanna kill it?" prompt after a few seconds when running
	// the application
	sf::Event event;
	while(_window.pollEvent(event));
}

void GuiChat::handleSendButtonCallback()
{
	treatMessage(_messageEditBox->getText());
	_messageEditBox->setText("");
}

void GuiChat::display(const std::string& name, const std::string& message)
{
	//if(_friendPresence.load())
	if(_chatBox)
		_chatBox->addLine(name + ": " + message);
	//else
		// Display the line in grey
		//_chatBox->addLine(name + ": " + message, sf::Color(127, 127, 127));
}

void GuiChat::output()
{
	while(_running)
	{
		// Draw the GUI
		_window.clear(sf::Color::White);
		_gui.draw();
		_window.display();

		// Process SFML events
		sf::Event event;
		while(_window.pollEvent(event))
		{
			// Close window, exit
			if(event.type == sf::Event::Closed)
			{
				_window.close();
				endDiscussion();
			}

			// Give the event to the GUI, to bring menus alive
			_gui.handleEvent(event);
		}
	}
}

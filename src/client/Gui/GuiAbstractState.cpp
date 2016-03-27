// std-C++ headers
#include <string>
#include <cassert>
// External headers
#include <SFML/Window/Event.hpp>
#include <TGUI/Widgets/MessageBox.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"


GuiAbstractState::GuiAbstractState(Context& context):
	AbstractState(context)
{
}

GuiAbstractState::~GuiAbstractState()
{
	for(auto& widget : _rootWidgets)
		widget->getParent()->remove(widget);
}

void GuiAbstractState::onPush()
{
	for(auto& widget : _rootWidgets)
		widget->hide();
	AbstractState::onPush();
}

void GuiAbstractState::onPop()
{
	for(auto& widget : _rootWidgets)
		widget->show();
	AbstractState::onPop();
}

void GuiAbstractState::display()
{
	_context.window->clear(sf::Color::White);
	_context.gui->draw();
	_context.window->display();
}

void GuiAbstractState::handleInput()
{
	// Process SFML events
	sf::Event event;
	while(_context.window->pollEvent(event))
	{
		// Close window, exit
		if(event.type == sf::Event::Closed)
		{
			_context.window->close();
			_context.stateStack->clear();
		}

		// Give the event to the GUI, to bring menus alive
		_context.gui->handleEvent(event);
	}
}

void GuiAbstractState::displayMessage(const std::string& message)
{
	static const std::string okButtonText{"Ok"};
	tgui::MessageBox::Ptr messageBox{std::make_shared<tgui::MessageBox>()};

	// Set up the message box
	messageBox->setText(message);
	messageBox->addButton(okButtonText);
	messageBox->getRenderer()->setTitleBarColor({127, 127, 127});
	_context.gui->add(messageBox);
	messageBox->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(messageBox)) / 2,
			(tgui::bindHeight(*_context.gui) - tgui::bindHeight(messageBox)) / 2);

	// Make the "Ok" button closing the message box
	// Note: do not try to pass messageBox as reference, since this lambda will
	// be stored elsewhere, the reference will become invalid when we'll go out
	// of the scope of this method! We must pass it by value.
	messageBox->connect("buttonPressed", [messageBox](const sf::String& buttonName)
	{
		if(buttonName == okButtonText)
			messageBox->destroy();
	});
}

void GuiAbstractState::registerRootWidgets(std::vector<tgui::Widget::Ptr>&& widgets)
{
	// Move to content of widgets to the back of _rootWidgets
	std::move(widgets.begin(), widgets.end(), std::back_inserter(_rootWidgets));
}

void GuiAbstractState::makeTitle(const std::string& title, unsigned int size, float y)
{
	_titleLabel.reset(new tgui::Label());
	_titleLabel->setText(title);
	_titleLabel->setTextSize(size);
	_titleLabel->setPosition((tgui::bindWidth(*_context.gui) - tgui::bindWidth(_titleLabel)) / 2.f, y);
	_context.gui->add(_titleLabel);
	registerRootWidgets({_titleLabel});
}

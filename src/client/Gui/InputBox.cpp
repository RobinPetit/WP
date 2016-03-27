#include "client/Gui/InputBox.hpp"

const std::string InputBox::okButtonText{"Ok"};

InputBox::InputBox():
	tgui::ChildWindow(),
	_label{std::make_shared<tgui::Label>()},
	_inputBar{std::make_shared<tgui::EditBox>()},
	_okButton{std::make_shared<tgui::Button>()},
	_grid{std::make_shared<tgui::Grid>()}
{
	// create buttons and their text
	_okButton->setText(okButtonText);

	// connect buttons to actions
	_okButton->connect("Pressed", [this]()
	{
		_callback(_inputBar->getText());
		destroy();
	});

	_grid->addWidget(_label, 0, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::UpperLeft) ;
	_grid->addWidget(_inputBar, 1, 0, {0, 0, 0, 0}, tgui::Grid::Alignment::UpperRight);
	_grid->addWidget(_okButton, 2, 0);
	add(_grid);
}

void InputBox::setLabelText(const std::string& newContent)
{
	_label->setText(newContent);
	reload();
}

void InputBox::setGridPosition(const tgui::Layout& x, const tgui::Layout& y)
{
	_grid->setPosition(x, y);
}

void InputBox::setGridSize(const tgui::Layout& x, const tgui::Layout& y)
{
	_grid->setSize(x, y);
}

void InputBox::setCallback(const std::function<void(const std::string&)>& callback)
{
	_callback = callback;
}

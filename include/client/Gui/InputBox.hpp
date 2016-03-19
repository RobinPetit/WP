#ifndef _INPUT_BOX_HPP_
#define _INPUT_BOX_HPP_

// std-C++ headers
#include <string>
#include <memory>
#include <functional>
// External headers
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Grid.hpp>
#include <TGUI/Layout.hpp>

class InputBox : public tgui::ChildWindow
{
public:
	////////// Alisases

	typedef std::shared_ptr<InputBox> Ptr;
	typedef std::shared_ptr<const InputBox> ConstPtr;

	////////// Methods

	InputBox();

	void setLabelText(const std::string& newContent);

	void setGridPosition(const tgui::Layout& x, const tgui::Layout& y);

	void setGridSize(const tgui::Layout& x, const tgui::Layout& y);

	void setCallback(const std::function<void(const std::string&)>& callback);

private:
	/////////// Attributes

	// tgui widgets

	tgui::Label::Ptr _label;
	tgui::EditBox::Ptr _inputBar;
	tgui::Button::Ptr _okButton;

	tgui::Grid::Ptr _grid;

	// elementary-typed variables

	std::function<void(const std::string& )> _callback;

	////////// Static members

	static const std::string okButtonText;

	////////// Private methods

	void handleOkPressed();

	void handleCancelPressed();
};

#endif  // _INPUT_BOX_HPP_

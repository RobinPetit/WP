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

/// An InputBox is a widget a bit like a MessageBox but with an EditBox
/// as well. The aim of a MessageBox is to display a message and the aim
/// of an InputBox is to receive a message from the user.
///
/// An InputBox contains a title, a label to explain to the user what
/// he is expected to enter, an edit box for the user to write down text
/// and a "Ok" button to finish the process.
///
/// \code
/// 	tgui::Gui *gui{/*...*/};
///	// ...
///	InputBox::Ptr input{std::make_shared<InputBox>()};
///	input->setTitle("My title");
///	input->setLabelText("Enter something");
///	input->setCallback([](const std::string& editBoxContent)
/// 	{
///		std::cout << "The content of the edit box when 'Ok' had "
///		             "been pressed was " << editBoxContent << '\n';
///	});
///	// Do think of setting the callback **before** to add the widget otherwise
///	// undefined behaviour is applied!
///	gui->add(input);
/// \endcode
class InputBox : public tgui::ChildWindow
{
public:
	////////// Alisases

	typedef std::shared_ptr<InputBox> Ptr;
	typedef std::shared_ptr<const InputBox> ConstPtr;

	////////// Methods

	/// Constructor
	InputBox();

	/// Change the label explaining what to write in the edit box
	/// \param newContent A string with the text to set in the label
	void setLabelText(const std::string& newContent);

	/// Change the size of the widget occupied by the content by changing
	/// the tgui::Grid dimention (and every internal widget is in the grid)
	/// \param x Length on the X axis
	/// \param y Length on the Y axis
	void setGridSize(const tgui::Layout& x, const tgui::Layout& y);

	/// Change the position of the tgui::Grid
	/// \param x Where to place the upper left corner on the X axis
	/// \param y Where to place the upper left corner on the Y axis
	void setGridPosition(const tgui::Layout& x, const tgui::Layout& y);

	/// Set the callback which is called when the Ok button is pressed.
	/// This method MUST be called before the widget is used to avoid risks
	/// of widget calling a callback never set.
	/// The callback must be typed `void callback(const std::string& str);`
	/// Where `str` is he content of the EditBox when the "Ok" button is pressed.
	/// \param callback The function to call at "Ok" button pressing
	void setCallback(const std::function<void(const std::string&)>& callback);

private:
	/////////// Attributes

	// tgui widgets

	tgui::Label::Ptr _label;       ///< The label describing what to enter
	tgui::EditBox::Ptr _inputBar;  ///< The edit box to write in
	tgui::Button::Ptr _okButton;   ///< The button to press to validate the input (and call the callback)

	tgui::Grid::Ptr _grid;         ///< The grid used to place the widgets

	// elementary-typed variables

	std::function<void(const std::string& )> _callback;  ///< The callback to call on "Ok" pressing

	////////// Static members

	static const std::string okButtonText;
};

#endif  // _INPUT_BOX_HPP_

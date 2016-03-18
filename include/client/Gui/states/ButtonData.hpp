#ifndef _BUTTON_DATA_HPP_
#define _BUTTON_DATA_HPP_

// External headers
#include <TGUI/Widgets/Button.hpp>
// std-C++ headers
#include <string>

template <typename GuiState>
struct ButtonData
{
private:
	typedef void (GuiState::*Callback)();

public:
	Callback callback;
	std::string text;
	tgui::Button::Ptr button = std::make_shared<tgui::Button>();
};

#endif  // _BUTTON_DATA_HPP_

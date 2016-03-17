// std-C++ headers
#include <iostream>
// WidzardPoker headers
#include "client/Gui/states/GuiMainMenuState.hpp"

GuiMainMenuState::GuiMainMenuState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractMainMenuState(context)
{

}

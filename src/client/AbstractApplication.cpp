#include "client/AbstractApplication.hpp"

AbstractApplication::AbstractApplication(bool Gui):
	_client{Gui},
	_stateStack{_context},
	_context{&_client, &_stateStack, nullptr, nullptr}
{
}

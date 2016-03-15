#include "client/AbstractApplication.hpp"

AbstractApplication::AbstractApplication():
	_client{},
	_stateStack{_context},
	_context{&_client, &_stateStack, nullptr}
{
}

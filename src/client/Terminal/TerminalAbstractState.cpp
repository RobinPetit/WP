// std-C++ headers
#include <string>
#include <cassert>
// WizardPoker headers
#include "client/Terminal/TerminalAbstractState.hpp"


TerminalAbstractState::TerminalAbstractState(Context& context):
	AbstractState(context)
{

}

void TerminalAbstractState::display()
{
	displaySeparator("Options", '*'); //separate options

	for(size_t i{1}; i < _actions.size(); ++i)
		std::cout << "  " << i << ". " << _actions[i].first << "\n";
	assert(_actions.size() > 0);
	// Display the menu entry 0 (which should be 'quit' or something like this) at last
	// because this is strange to have 'quit' as first possibility in a menu
	std::cout << "  0. " << _actions[0].first << "\n";
}

void TerminalAbstractState::displaySeparator(const std::string& separatorText, const char& separator)
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); //read size of terminal into ws
	int charRepeat = (ws.ws_col - separatorText.size() - 2)/2;
	std::cout 	<< std::string(charRepeat, separator)
				<< " " << separatorText << " "
				<< std::string(ws.ws_col - charRepeat - separatorText.size() - 2, separator)
				<< std::endl;
}

void TerminalAbstractState::displayEntry(const std::string& entryText, char sep, std::size_t entryLevel)
{
	std::cout << std::string(entryLevel*4 + 2, ' ') << sep << " " << entryText << std::endl;
}

void TerminalAbstractState::displayNumberedEntry(const std::string& entryText, int entryNumber, std::size_t entryLevel)
{
	int spacesCount = entryLevel*4 + 2 - std::to_string(entryNumber).size();
	if (spacesCount<0)
		spacesCount=0;
	std::cout << std::string(spacesCount, ' ') << entryNumber << ". " << entryText << std::endl;
}

void TerminalAbstractState::displayCard(CardId id, bool displayId)
{
	auto cardData = _context.client->getCardData(id);
	assert(cardData->isCreature() or cardData->isSpell());

	if(cardData->isCreature())
	{
		const ClientCreatureData* castedCardData{dynamic_cast<const ClientCreatureData*>(cardData)};
		assert(castedCardData != nullptr);
		if (displayId)
			displayNumberedEntry(std::string(castedCardData->getName()) + " (creature)", static_cast<int>(id));
		else
			displayEntry(std::string(castedCardData->getName()) + " (creature)");
		displayEntry(std::string("cost: ") + std::to_string(castedCardData->getCost())
					+ ", attack: " + std::to_string(castedCardData->getAttack())
					+ ", health: " + std::to_string(castedCardData->getHealth())
					+ ", shield: " + std::to_string(castedCardData->getShield())
					+ "-" + std::to_string(castedCardData->getShieldType()), '-', 1);
		displayEntry(castedCardData->getDescription(), '-', 1);
	}
	else
	{
		const ClientSpellData* castedCardData{dynamic_cast<const ClientSpellData*>(cardData)};
		assert(castedCardData != nullptr);

		if (displayId)
			displayNumberedEntry(std::string(castedCardData->getName()) + " (spell)", static_cast<int>(id));
		else
			displayEntry(std::string(castedCardData->getName()) + " (spell)");
		displayEntry(std::string("cost: ") + std::to_string(castedCardData->getCost()), '-', 1);
		displayEntry(castedCardData->getDescription(), '-', 1);
	}
}

void TerminalAbstractState::handleInput()
{
	//TODO : change try/catch block for something more precise, this does NOT only catch input errors !
	try
	{
		// Get the user input
		std::string input;
		std::cout << "Choose an option: ";
		std::getline(std::cin, input);
		// Cat to a number
		const int intInput{std::stoi(input)};
		// Call the method at index intInput
		// std::vector::at throws std::out_of_range if intInput is out of bounds
		_actions.at(intInput).second();
	}
	catch(const std::logic_error& e)  // Also catches std::out_of_range
	{
		std::cout << "Wrong input!\n";
		waitForEnter();
	}
}

void TerminalAbstractState::waitForEnter()
{
	std::cout << "Press Enter to continue...";
	std::string tmp;
	std::getline(std::cin, tmp);
}

void TerminalAbstractState::displayMessage(const std::string& message)
{
	std::cout << message << std::endl;
}

std::size_t TerminalAbstractState::askForNumber(std::size_t from, std::size_t to)
{
	std::string input;
	do
	{
		std::getline(std::cin, input);
	} while(input == "");
	// Get a number from the user input
	const std::size_t intInput{std::stoul(input)};
	if(intInput < from or intInput >= to)
		throw std::out_of_range("Index out of range.");
	return intInput;
}

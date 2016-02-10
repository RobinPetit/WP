#ifndef _CARDS_COLLECTION_STATE_CLIENT_HPP
#define _CARDS_COLLECTION_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class CardsCollectionState : public AbstractState
{
    public:
        /// Constructor.
        CardsCollectionState(StateStack& stateStack);

        /// The display function.
        /// It must do all things related to drawing or printing stuff on the screen.
        virtual void display() override;

        /// The event handling function.
        virtual void handleInput(const std::string& input) override;

    private:
        void backMainMenu();
        std::vector<std::string> _cards;///< The list of cards, use the class Card instead of std::string when it will be available

};

#endif// _CARDS_COLLECTION_STATE_CLIENT_HPP


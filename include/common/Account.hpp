#ifndef _ACCOUNT_COMMON_HPP
#define _ACCOUNT_COMMON_HPP

#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

class Account
{
	public:
		typedef std::size_t ID;

		std::string _userName;
		std::string _password;
		unsigned int _wonGames;
		unsigned int _lostGames;
		CardsCollection _cardsCollection;
		std::vector<Deck> _decks;
		std::set<ID> _friends;
		ID _id;
};

#endif  // _ACCOUNT_COMMON_HPP

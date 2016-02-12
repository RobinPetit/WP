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
		unsigned int _wonMatches;
		unsigned int _lostMatches;
		CardsCollection _cardsCollection;
		std::map<std::string, Deck> _decks;
		std::set<ID> _friends;
};

#endif  // _ACCOUNT_COMMON_HPP

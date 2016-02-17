set(SOURCES
	"Deck.cpp"
	"CardsCollection.cpp"
	"NotConnectedException.cpp"
	"Terminal.cpp"
	"ini/IniFile.cpp"
	"sockets/TransferType.cpp"
	)

add_library(${COMMON_NAME} "${SOURCES}")

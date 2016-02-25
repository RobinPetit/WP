set(SOURCES
	"Deck.cpp"
	"CardsCollection.cpp"
	"NotConnectedException.cpp"
	"Terminal.cpp"
	"ini/IniFile.cpp"
	"sockets/TransferType.cpp"
	"sockets/PacketOverload.cpp"
	"Database.cpp"
	)

add_library(${COMMON_NAME} ${SOURCES})

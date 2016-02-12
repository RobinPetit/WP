set(SOURCES
	"Deck.cpp"
	"CardsCollection.cpp"
	)

set(COMMON_NAME "${PROJECT_NAME}_common")

add_library(${COMMON_NAME} "${SOURCES}")

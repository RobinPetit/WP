set(SOURCES	"client.cpp"
		"ClientCardData.cpp"
		"ClientCreature.cpp"
		"ClientSpell.cpp"
		"ClientDatabase.cpp"
		# sockets
		"sockets/Client.cpp"
		"NonBlockingInput.cpp"
		# states
		"AbstractState.cpp"
		"Terminal/TerminalAbstractState.cpp"
		"StateStack.cpp"
		"states/AbstractCardsCollectionState.cpp"
		"states/AbstractDecksManagementState.cpp"
		"states/AbstractFriendsManagementState.cpp"
		"states/AbstractLadderState.cpp"
		"states/AbstractAchievementState.cpp"
		"states/AbstractLobbyState.cpp"
		"states/AbstractMainMenuState.cpp"
		"states/AbstractHomeState.cpp"
		"Terminal/states/TerminalCardsCollectionState.cpp"
		"Terminal/states/TerminalDecksManagementState.cpp"
		"Terminal/states/TerminalFriendsManagementState.cpp"
		"Terminal/states/TerminalLadderState.cpp"
		"Terminal/states/TerminalAchievementState.cpp"
		"Terminal/states/TerminalLobbyState.cpp"
		"Terminal/states/TerminalMainMenuState.cpp"
		"Terminal/states/TerminalHomeState.cpp"
		# Game
		"AbstractGame.cpp"
		"Terminal/TerminalGame.cpp"
	)

set(CLIENT_NAME "${PROJECT_NAME}_client")

# Create the executable
add_executable(${CLIENT_NAME} ${SOURCES})

# Link the executable to the libraries
target_link_libraries(${CLIENT_NAME} ${COMMON_NAME} ${EXTERNAL_LIBRARIES})

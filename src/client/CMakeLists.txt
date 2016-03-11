set(SOURCES	"client.cpp"
		"ClientDatabase.cpp"
		# sockets
		"sockets/Client.cpp"
		"NonBlockingInput.cpp"
		# states
		"AbstractState.cpp"
		"StateStack.cpp"
		"states/CardsCollectionState.cpp"
		"states/DecksManagementState.cpp"
		"states/FriendsManagementState.cpp"
		"states/LadderState.cpp"
		"states/LobbyState.cpp"
		"states/MainMenuState.cpp"
		"states/HomeState.cpp"
		# Game
		"AbstractGame.cpp"
		"Terminal/TerminalGame.cpp"
	)

set(CLIENT_NAME "${PROJECT_NAME}_client")

# Create the executable
add_executable(${CLIENT_NAME} ${SOURCES})

# Link the executable to the libraries
target_link_libraries(${CLIENT_NAME} ${COMMON_NAME} ${EXTERNAL_LIBRARIES})

add_custom_command(
	TARGET ${CLIENT_NAME}
		PRE_BUILD
		COMMAND ./initdatabase.sh
		WORKING_DIRECTORY ../../../prebuildscript/
	)

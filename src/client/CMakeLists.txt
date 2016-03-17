set(SOURCES
		# Unspecialized files
		"client.cpp"
		"ClientDatabase.cpp"
		"AbstractApplication.cpp"
		## sockets
		"sockets/Client.cpp"
		"NonBlockingInput.cpp"
		## states
		"AbstractState.cpp"
		"StateStack.cpp"
		"states/AbstractCardsCollectionState.cpp"
		"states/AbstractDecksManagementState.cpp"
		"states/AbstractFriendsManagementState.cpp"
		"states/AbstractLadderState.cpp"
		"states/AbstractLobbyState.cpp"
		"states/AbstractMainMenuState.cpp"
		"states/AbstractHomeState.cpp"
		## Game
		"AbstractGame.cpp"

		# Terminal files
		"Terminal/TerminalApplication.cpp"
		## States
		"Terminal/TerminalAbstractState.cpp"
		"Terminal/states/TerminalCardsCollectionState.cpp"
		"Terminal/states/TerminalDecksManagementState.cpp"
		"Terminal/states/TerminalFriendsManagementState.cpp"
		"Terminal/states/TerminalLadderState.cpp"
		"Terminal/states/TerminalLobbyState.cpp"
		"Terminal/states/TerminalMainMenuState.cpp"
		"Terminal/states/TerminalHomeState.cpp"
		## Game
		"Terminal/TerminalGame.cpp"

		# GUI files
		"Gui/GuiApplication.cpp"
		## States
		"Gui/GuiAbstractState.cpp"
		"Gui/states/GuiHomeState.cpp"
		"Gui/states/GuiMainMenuState.cpp"
	)

set(CLIENT_NAME "${PROJECT_NAME}_client")

# Create the executable
add_executable(${CLIENT_NAME} ${SOURCES})

# Link the executable to the libraries
target_link_libraries(${CLIENT_NAME} ${COMMON_NAME} ${EXTERNAL_LIBRARIES})

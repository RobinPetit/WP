add_library(${CLIENT_LIB_NAME}
		# sockets
		"sockets/Client.cpp"
		"sockets/ChatListening.cpp"
		# states
		"AbstractState.cpp"
		"StateStack.cpp"
		"states/CardsCollectionState.cpp"
		"states/DecksManagementState.cpp"
		"states/FriendsManagementState.cpp"
		"states/LadderState.cpp"
		"states/MainMenuState.cpp"
		"states/HomeState.cpp"
	)

set(CLIENT_NAME "${PROJECT_NAME}_client")

# Create the executable
add_executable(${CLIENT_NAME} "client.cpp")

# Link the executable to the libraries
target_link_libraries(${CLIENT_NAME}
		${CLIENT_LIB_NAME}
		${COMMON_LIB_NAME}
		${EXTERNAL_LIBRARIES}
		)


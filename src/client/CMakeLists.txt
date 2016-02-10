set(SOURCES
    "client.cpp"
    # states
    "AbstractState.cpp"
    "StateStack.cpp"
    "states/CardsCollectionState.cpp"
    "states/DecksManagementState.cpp"
    "states/FriendsManagementState.cpp"
    "states/LadderState.cpp"
    "states/MainMenuState.cpp"
    "states/HomeState.cpp"
    # sockets
    #~"sockets/Connection.cpp"
    )

set(CLIENT_NAME "${PROJECT_NAME}_client")

# Create the executable
add_executable(${CLIENT_NAME} "${SOURCES}")

# Link the executable to the libraries
target_link_libraries(${CLIENT_NAME} ${EXTERNAL_LIBRARIES})

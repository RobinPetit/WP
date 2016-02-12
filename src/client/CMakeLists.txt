set(SOURCES
		"client.cpp"
		# sockets
		"sockets/Client.cpp"
		"sockets/ChatListening.cpp"
		# common
		"../common/sockets/TransferType.cpp"
		"../common/ConfigParser.cpp"
		"../common/Terminal.cpp"
	)

set(CLIENT_NAME "${PROJECT_NAME}_client")

# Create the executable
add_executable(${CLIENT_NAME} "${SOURCES}")

# Link the executable to the libraries
target_link_libraries(${CLIENT_NAME} ${EXTERNAL_LIBRARIES} pthread)


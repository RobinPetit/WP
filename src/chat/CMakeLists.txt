set(SOURCES
		"chat.cpp"
		# common
		"../common/sockets/TransferType.cpp"
	)

set(CHAT_NAME "${PROJECT_NAME}_chat")

add_executable(${CHAT_NAME} "${SOURCES}")

# Link the executable to the libraries
target_link_libraries(${CHAT_NAME} ${EXTERNAL_LIBRARIES} pthread)

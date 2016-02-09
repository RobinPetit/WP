set(SOURCES
		"server.cpp"
		# sockets
		"sockets/Server.cpp"
		# common
		"../common/sockets/TransferType.cpp"
	)

set(SERVER_NAME "${PROJECT_NAME}_server")

add_executable(${SERVER_NAME} "${SOURCES}")

target_link_libraries(${SERVER_NAME} ${EXTERNAL_LIBRARIES})

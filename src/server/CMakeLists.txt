set(SOURCES
		"server.cpp"
		"ServerDatabase.cpp"
		# sockets
		"sockets/Server.cpp"
		# common
	)

set(SERVER_NAME "${PROJECT_NAME}_server")

add_executable(${SERVER_NAME} ${SOURCES})

target_link_libraries(${SERVER_NAME} ${COMMON_NAME} ${EXTERNAL_LIBRARIES})

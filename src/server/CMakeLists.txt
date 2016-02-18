add_library(${SERVER_LIB_NAME}
		# sockets
		"sockets/Server.cpp"
	)

set(SERVER_NAME "${PROJECT_NAME}_server")

add_executable(${SERVER_NAME} "server.cpp")

target_link_libraries(${SERVER_NAME}
		${SERVER_LIB_NAME}
		${COMMON_LIB_NAME}
		${EXTERNAL_LIBRARIES}
		pthread
	)

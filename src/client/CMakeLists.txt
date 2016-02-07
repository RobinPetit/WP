set(SOURCES
	"client.cpp"
	# sockets
	"sockets/Connection.cpp"
	)

add_executable("${PROJECT_NAME}_client" "${SOURCES}")

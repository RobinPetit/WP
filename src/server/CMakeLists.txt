set(SOURCES
		"server.cpp"
		"Board.cpp"
		"Card.cpp"
		"Creature.cpp"
		"Spell.cpp"
		"Player.cpp"
		"Constraints.cpp"
		# sockets
		"sockets/Server.cpp"
		# common
	)

set(SERVER_NAME "${PROJECT_NAME}_server")

add_executable(${SERVER_NAME} "${SOURCES}")

target_link_libraries(${SERVER_NAME} ${EXTERNAL_LIBRARIES} ${COMMON_NAME} pthread)

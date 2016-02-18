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
		"../common/sockets/TransferType.cpp"
		"../common/ini/IniFile.cpp"
	)

set(SERVER_NAME "${PROJECT_NAME}_server")

add_executable(${SERVER_NAME} "${SOURCES}")

target_link_libraries(${SERVER_NAME} ${EXTERNAL_LIBRARIES} pthread)

set(SOURCES
		"server.cpp"
		"ServerDatabase.cpp"
		"Board.cpp"
		"Card.cpp"
		#"CardData.cpp"  # contains the cards declaration
		"Creature.cpp"
		"Spell.cpp"
		"Player.cpp"
		"Constraints.cpp"
		# sockets
		"sockets/Server.cpp"
		"sockets/GameThread.cpp"
	)

set(SERVER_NAME "${PROJECT_NAME}_server")

add_executable(${SERVER_NAME} ${SOURCES})

target_link_libraries(${SERVER_NAME} ${COMMON_NAME} ${EXTERNAL_LIBRARIES})

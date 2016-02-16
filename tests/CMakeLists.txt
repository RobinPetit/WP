# Find the Boost unit test package
find_package(Boost 1.57 COMPONENTS unit_test_framework REQUIRED)

# Add the library to the linkage
set(EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pthread)

# Specify to Boost that it is dynamically linked
add_definitions(-DBOOST_TEST_DYN_LINK)

set(TESTS_SOURCES
	"common/ini/IniFileTests.cpp"
	"common/NotConnectedExceptionTests.cpp"
	"common/sockets/TransferTypeTests.cpp"
	"common/TerminalTests.cpp"
	"client/StateStackTests.cpp"
	"client/sockets/ChatListeningTests.cpp"
	"client/sockets/ClientTests.cpp"
	"client/states/HomeStateTests.cpp"
	"client/states/LadderStateTests.cpp"
	"client/states/MainMenuStateTests.cpp"
	"client/states/DecksManagementStateTests.cpp"
	"client/states/CardsCollectionStateTests.cpp"
	"client/states/FriendsManagementStateTests.cpp"
	"client/AbstractStateTests.cpp"
	"main.cpp"
	"server/sockets/ServerTests.cpp"
	)

set(APP_SOURCES
	"../src/common/ini/IniFile.cpp"
	"../src/common/sockets/TransferType.cpp"
	"../src/common/Terminal.cpp"
	"../src/common/NotConnectedException.cpp"
	"../src/client/StateStack.cpp"
	"../src/client/sockets/Client.cpp"
	"../src/client/sockets/ChatListening.cpp"
	"../src/client/AbstractState.cpp"
	"../src/client/states/MainMenuState.cpp"
	"../src/client/states/DecksManagementState.cpp"
	"../src/client/states/HomeState.cpp"
	"../src/client/states/CardsCollectionState.cpp"
	"../src/client/states/FriendsManagementState.cpp"
	"../src/client/states/LadderState.cpp"
	"../src/server/sockets/Server.cpp"
	)
	

set(TESTS_NAME ${PROJECT_NAME}_tests)

add_executable(${TESTS_NAME} ${TESTS_SOURCES} ${APP_SOURCES})

target_link_libraries(${TESTS_NAME} ${EXTERNAL_LIBRARIES})


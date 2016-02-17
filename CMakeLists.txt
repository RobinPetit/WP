cmake_minimum_required(VERSION 3.0.2)

project("WizardPoker")

#Set path to Find* scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Find dependencies
find_package(SFML 2.1 COMPONENTS system network REQUIRED)

# Set the include directory of the project
include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${SFML_INCLUDE_DIR}"
	)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

set(CMAKE_CXX_COMPILER "g++-5")
# Set compiler flags (they must be in one line)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -Wextra -pedantic -pedantic-errors -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Winline -Wunreachable-code -Wmissing-declarations")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")

# This variable contains the name of all libraries that has to be linked
set(EXTERNAL_LIBRARIES ${SFML_LIBRARIES} pthread)

# These are the names of the libs build from the common/client/server sources.
# Libs are needed in order to compile one time source files for normal release
# and unit test executable.
set(COMMON_LIB_NAME ${PROJECT_NAME}_common_lib)
set(CLIENT_LIB_NAME ${PROJECT_NAME}_client_lib)
set(SERVER_LIB_NAME ${PROJECT_NAME}_server_lib)
# Uncomment the following line as soon there is other source files that
# chat.cpp to build for the chat
#set(CHAT_LIB_NAME ${PROJECT_NAME}_chat_lib)

# Tell to cmake to explore src/ to build both the server and the client
add_subdirectory(src)

# And build the SRD
add_subdirectory(SRD)

# Also build unit tests
add_subdirectory(tests)

# Move the executables created in src/client/ and src/server/
# to the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

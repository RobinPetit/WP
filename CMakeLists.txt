cmake_minimum_required(VERSION 3.2)

project("WizardPoker")

#Set path to Find* scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Find dependencies
find_package(SFML 2.3 COMPONENTS system network REQUIRED)

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
set(EXTERNAL_LIBRARIES ${SFML_LIBRARIES})

# Tell to cmake to explore src/ to build both the server and the client
add_subdirectory(src)
# And build the SRD
add_subdirectory(SRD)

# Move the executables created in src/client/ and src/server/
# to the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


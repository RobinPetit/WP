cmake_minimum_required(VERSION 3.2)

project("WizardPoker")

# Set the include directory of the project
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

set(CMAKE_CXX_COMPILER "g++-5")
# Set compiler flags (they must be in one line)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -Wextra -pedantic -pedantic-errors -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Winline -Wunreachable-code -Wmissing-declarations")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")

# Tell to cmake to explore src/ to build both the server and the client
add_subdirectory(src)
add_subdirectory(SRD)

# Create an executable from the object compiled in src/
# add_executable(${PROJECT_NAME} "$<TARGET_OBJECTS:WP>")


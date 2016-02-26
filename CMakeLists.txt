cmake_minimum_required(VERSION 3.0.2)

project("WizardPoker")

# Set path to Find* scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Tell to SFML whether we compile in static mode if we build for the NO4
# set(SFML_STATIC_LIBRARIES TRUE)

# Find dependencies
find_package(SFML 2.1 COMPONENTS network system REQUIRED)
find_package(SQLite3 REQUIRED)

# Set the include directory of the project
include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${SFML_INCLUDE_DIR}"
	"${SQLITE3_INCLUDE_DIRS}"
	)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/binNO4/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

set(CMAKE_CXX_COMPILER "g++-5")
# Set compiler flags (they must be in one line)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -Wextra -pedantic -pedantic-errors -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Winline -Wunreachable-code -Wmissing-declarations -Wconversion -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wzero-as-null-pointer-constant")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")

# This variable contains the name of all libraries that has to be linked
set(EXTERNAL_LIBRARIES ${SFML_LIBRARIES} ${SQLITE3_LIBRARIES} pthread)

# If we build in static (for the NO4), SFML expects its dependencies to be linked
# set(EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} ${SFML_DEPENDENCIES})

# Tell to cmake to explore src/ to build both the server and the client
add_subdirectory(src)

add_custom_target(srd
		COMMAND make -C ${PROJECT_SOURCE_DIR}/SRD/ srd.pdf
		COMMAND cp ${PROJECT_SOURCE_DIR}/SRD/srd.pdf ${PROJECT_SOURCE_DIR}/srd.pdf
	)
add_custom_target(cleansrd COMMAND make -C ${PROJECT_SOURCE_DIR}/SRD/ mrproper)

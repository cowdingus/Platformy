cmake_minimum_required(VERSION 3.18.4)

project(Platformy)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS code/*.cpp)

add_executable("${PROJECT_NAME}" ${SOURCES})

add_compile_options(-Wall -Wextra -pedantic -Werror -Og)

# Set to use C++17
set_target_properties(
	"${PROJECT_NAME}"
	PROPERTIES
	CXX_STANDARD 17
	)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

target_include_directories(
	"${PROJECT_NAME}" PRIVATE
	header
	)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
	sfml-graphics
	sfml-window
	sfml-system
	)

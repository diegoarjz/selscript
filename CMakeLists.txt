project("sel_script" LANGUAGES CXX)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("gtest")

find_package(Boost COMPONENTS program_options)
if (Boost_FOUND)
    message(STATUS "Boost was found")
else()
	message(STATUS "Boost not found")
endif()

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)

enable_testing()
add_subdirectory(test)

project("sel_script" LANGUAGES CXX)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

find_package(Boost)
if (Boost_FOUND)
    message(STATUS "Boost was found")
else()
	message(STATUS "Boost not found")
endif()

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)

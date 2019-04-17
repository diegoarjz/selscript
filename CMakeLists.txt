project("sel_script" LANGUAGES CXX)
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("gtest")

find_package(Boost COMPONENTS program_options)

# Export compile comands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)

option(SELSCRIPT_BUILD_TESTS "Build tests" ON)
if (SELSCRIPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

set(INTERPRETER_SOURCES
    "interpreter.cpp"
    "interpreter_visitor.cpp"
    "symbol_table.cpp"
)

add_library(interpreter ${INTERPRETER_SOURCES})

target_include_directories(
    interpreter
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    interpreter
    PRIVATE
        cxx_std_14
)

target_compile_options(
    interpreter
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    interpreter
    PRIVATE
        intermediate
        value
)

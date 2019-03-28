set(PARSER_SOURCES
    "grammar_helpers.cpp"
    "parser.cpp"
)

add_library(parser ${PARSER_SOURCES})

target_include_directories(
    parser
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    parser
    PRIVATE
        cxx_std_14
)

target_compile_options(
    parser
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    parser
    PRIVATE
        intermediate
)

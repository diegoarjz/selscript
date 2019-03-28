set(INTERMEDIATE_SOURCES
    "ast.cpp"
    "ast_printer.cpp"
)

add_library(intermediate ${INTERMEDIATE_SOURCES})

target_include_directories(
    intermediate
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    intermediate
    PRIVATE
        cxx_std_14
)

target_compile_options(
    intermediate
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)


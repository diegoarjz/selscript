set(SEL_SCRIPT_SOURCES
    "ast.cpp"
    "base_value.cpp"
    "boolean_value.cpp"
    "builtin_functions.cpp"
    "callable.cpp"
    "float_value.cpp"
    "functional.cpp"
    "null_object_value.cpp"
    "string_value.cpp"
    "symbol_table.cpp"
    "parser.cpp"
    "interpreter.cpp"
    "grammar_helpers.cpp"
    "ast_printer.cpp"
    )

add_library(selscript ${SEL_SCRIPT_SOURCES})

target_include_directories(
    selscript
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript
    PRIVATE
        cxx_std_14
)

target_compile_options(
    selscript
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    selscript
    PRIVATE
        Boost::program_options
)

# SSCRIPT EXECUTABLE
add_executable(sscript "main.cpp")
target_include_directories(
    sscript
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    sscript
    PRIVATE
        cxx_std_14
)

target_compile_options(
    sscript
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    sscript
    PRIVATE
        Boost::program_options
        selscript
)

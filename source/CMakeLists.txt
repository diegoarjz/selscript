set(SEL_SCRIPT_SOURCES
    "main.cpp"
    "ast.cpp"
    "symbol_table.cpp"
    "parser.cpp"
    "interpreter.cpp"
    "grammar_helpers.cpp"
    "ast_printer.cpp"
    )

add_executable(sscript ${SEL_SCRIPT_SOURCES})

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
)

set(SEL_SCRIPT_SOURCES
    "main.cpp"
    "symbol_table.cpp"
    "parser.cpp"
    "interpreter.cpp"
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

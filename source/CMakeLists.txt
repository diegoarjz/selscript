set(SEL_SCRIPT_SOURCES
    "version.cpp"
    )

add_subdirectory(intermediate)
add_subdirectory(value)
add_subdirectory(parser)
add_subdirectory(interpreter)

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
        parser
        value
        intermediate
        interpreter
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

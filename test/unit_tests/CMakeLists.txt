set(TESTS_SRCS
    "main.cpp"
    "grammar.cpp"
    "binding/argument_value.cpp"
    "binding/functional.cpp"
    "value/value_visitor.cpp"
)

add_executable(unit_tests ${TESTS_SRCS})

set_target_properties(
    unit_tests
    PROPERTIES 
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_include_directories(
    unit_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    unit_tests
    PRIVATE
        cxx_std_14
)

target_compile_options(
    unit_tests
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    unit_tests
    PRIVATE
        Boost::program_options
        selscript
        pthread
        gtest
)

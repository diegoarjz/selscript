set(VALUE_SOURCES
    "base_value.cpp"
    "boolean_value.cpp"
    "builtin_functions.cpp"
    "callable.cpp"
    "float_value.cpp"
    "functional.cpp"
    "null_object_value.cpp"
    "string_value.cpp"
)

add_library(value ${VALUE_SOURCES})

target_include_directories(
    value
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    value
    PRIVATE
        cxx_std_14
)

target_compile_options(
    value
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    value
    PRIVATE
)

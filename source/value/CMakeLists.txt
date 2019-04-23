set(VALUE_SOURCES
    "base_value.cpp"
    "boolean_value.cpp"
    "builtin_functions.cpp"
    "function.cpp"
    "class_value.cpp"
    "float_value.cpp"
    "integer_value.cpp"
    "instance_value.cpp"
    "null_object_value.cpp"
    "string_value.cpp"
    "type_info.cpp"
    "undefined_operator.cpp"
)

set(VALUE_PUBLIC_HEADERS
    "base_value.h"
    "boolean_value.h"
    "builtin_function.h"
    "builtin_functions.h"
    "callable.h"
    "class_value.h"
    "float_value.h"
    "function.h"
    "instance_value.h"
    "integer_value.h"
    "null_object_value.h"
    "string_value.h"
    "type_info.h"
    "undefined_operator.h"
    "value_visitor.h"
)

add_library(value OBJECT ${VALUE_SOURCES})

target_include_directories(
    value
    PRIVATE
        $<INSTALL_INTERFACE:selscript>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
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

include(GNUInstallDirs)
install(TARGETS value
    EXPORT value-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/value
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/value
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selscript/value)

install(EXPORT value-export
    FILE
        ValueTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
)

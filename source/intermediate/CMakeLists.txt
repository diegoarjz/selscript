set(INTERMEDIATE_SOURCES
    "ast.cpp"
    "ast_printer.cpp"
    "class_declaration.cpp"
    "get_expression.cpp"
    "set_expression.cpp"
)

set(INTERMEDIATE_PUBLIC_HEADERS
    "ast.h"
    "ast_printer.h"
    "ast_visitor.h"
    "class_declaration.h"
    "get_expression.h"
    "set_expression.h"
)

add_library(intermediate ${INTERMEDIATE_SOURCES})

target_include_directories(
    intermediate
    PRIVATE
        $<INSTALL_INTERFACE:selscript>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
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

include(GNUInstallDirs)
install(TARGETS intermediate
    EXPORT intermediate-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/intermediate
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/intermediate
)
install(FILES ${INTERMEDIATE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selscript/intermediate)

install(EXPORT intermediate-export
    FILE
        IntermediateTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
)

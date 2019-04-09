set(PARSER_SOURCES
    "grammar_helpers.cpp"
    "parser.cpp"
)

set(PARSER_PUBLIC_HEADERS
    "grammar.h"
    "grammar_helpers.h"
    "parser.h"
)

add_library(parser ${PARSER_SOURCES})

target_include_directories(
    parser
    PRIVATE
        $<INSTALL_INTERFACE:selscript>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
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

include(GNUInstallDirs)
install(TARGETS parser
    EXPORT parser-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/parser
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/parser
)
install(FILES ${PARSER_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selscript/parser)

install(EXPORT parser-export
    FILE
        ParserTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
)

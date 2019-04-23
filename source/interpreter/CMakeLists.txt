set(INTERPRETER_SOURCES
    "interpreter.cpp"
    "interpreter_visitor.cpp"
    "symbol_table.cpp"
)

set(INTERPRETER_PUBLIC_HEADERS
    "interpreter.h"
    "interpreter_visitor.h"
    "symbol_table.h"
)

add_library(interpreter OBJECT ${INTERPRETER_SOURCES}
        $<TARGET_OBJECTS:binding>
        $<TARGET_OBJECTS:intermediate>
        $<TARGET_OBJECTS:value>
)

target_include_directories(
    interpreter
    PRIVATE
        $<INSTALL_INTERFACE:selscript>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    interpreter
    PRIVATE
        cxx_std_14
)

target_compile_options(
    interpreter
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    interpreter
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS interpreter
    EXPORT interpreter-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/interpreter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/interpreter
)
install(FILES ${INTERPRETER_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selscript/interpreter)

install(EXPORT interpreter-export
    FILE
        InterpreterTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
)

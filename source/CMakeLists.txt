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
        $<INSTALL_INTERFACE:selscript>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
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


include(GNUInstallDirs)
install(TARGETS selscript
    EXPORT selscript-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/
)

install(EXPORT selscript-export
    FILE
        SelScriptTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
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

install(TARGETS sscript
    EXPORT sscript-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT sscript-export
    FILE
        SScriptTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
)

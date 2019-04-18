set(BINDING_SOURCES
    "argument_value.cpp"
    "functional.cpp"
)

set(BINDING_PUBLIC_HEADERS
    "argument_value.h"
    "functional.h"
)

add_library(binding ${BINDING_SOURCES})

target_include_directories(
    binding
    PRIVATE
        $<INSTALL_INTERFACE:selscript>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    binding
    PRIVATE
        cxx_std_14
)

target_compile_options(
    binding
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    binding
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS binding
    EXPORT binding-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/binding
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selscript/binding
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selscript/binding)

install(EXPORT binding-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        SelScript::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selscript/cmake/
)

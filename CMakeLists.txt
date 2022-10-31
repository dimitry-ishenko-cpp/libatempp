cmake_minimum_required(VERSION 3.1)
project(libatem++ VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DASIO_STANDALONE -DASIO_NO_DEPRECATED)
add_definitions(-DVERSION="${PROJECT_VERSION}")

set(HEADERS
    src/aux_bus.hpp
    src/aux_busses.hpp
    src/device.hpp
    src/input.hpp
    src/inputs.hpp
    src/me.hpp
    src/mes.hpp
    src/types.hpp
) # public headers, use SOURCES for private headers

set(OVERALL_HEADER src/atem++.hpp)

set(SOURCES
    src/aux_bus.cpp
    src/aux_busses.cpp
    src/cmd.hpp
    src/device.cpp
    src/input.cpp
    src/input_data.hpp
    src/inputs.cpp
    src/me.cpp
    src/mes.cpp
    src/packet.cpp src/packet.hpp
    src/session.cpp src/session.hpp
    src/utils.hpp
)

include(GNUInstallDirs)

########################
string(REGEX REPLACE "^lib" "" LIBRARY_NAME ${PROJECT_NAME})
set(PROJECT_NAME_DEV ${PROJECT_NAME}-dev)
set(CMAKE_INSTALL_DOCDIR_DEV ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME_DEV})

# shared library
add_library(shared SHARED ${HEADERS} ${SOURCES})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${LIBRARY_NAME})
set_target_properties(shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(shared PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS shared DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shared)

# static dev library and headers
add_library(static STATIC ${HEADERS} ${SOURCES})
set_target_properties(static PROPERTIES OUTPUT_NAME ${LIBRARY_NAME})

install(TARGETS static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT static)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_NAME} COMPONENT static)
install(FILES ${OVERALL_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT static)

cmake_minimum_required(VERSION 3.2.1)
project(default)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tests_base)

add_library(default STATIC default_server.c)
if (CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(default PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -g -fPIC")
endif (CMAKE_COMPILER_IS_GNUCXX)

if (APPLE)
    set(SYSTEM_LIBRARIES pthread)
elseif (UNIX)
    set(SYSTEM_LIBRARIES pthread)
else ()
    set(SYSTEM_LIBRARIES ws2_32)
endif ()


target_link_libraries(default PUBLIC Collector::Core Collector::HidApi open62541 ${SYSTEM_LIBRARIES})
add_library(Collector::Default ALIAS default)

set_target_properties(default
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(default_server default_server_entry.c)
target_link_libraries(default_server PUBLIC Collector::Core Collector::Default)

cmake_minimum_required(VERSION 3.2.1)
project(core)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tests_base)

add_library(core STATIC
        multithreading.c
        multithreading.h
        types/basic_types.c types/basic_types.h
        device/monitoring.c device/monitoring.h
        device/function.c device/function.h)
if (CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(core PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -g -fPIC")
endif (CMAKE_COMPILER_IS_GNUCXX)

if (APPLE)
    set(SYSTEM_LIBRARIES pthread)
elseif (UNIX)
    set(SYSTEM_LIBRARIES pthread)
else ()
    set(SYSTEM_LIBRARIES ws2_32)
endif ()


target_link_libraries(core PUBLIC Collector::HidApi open62541 ${SYSTEM_LIBRARIES})
add_library(Collector::Core ALIAS core)

set_target_properties(core
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (BUILD_TESTING)
    add_executable(tests_core tests_server_base.cpp tests_core_multithreading.cc types/tests_core_basic_types.cc device/tests_core_device.cc)
    target_link_libraries(tests_core core Collector::Test Collector::Default)
    if (WIN32)
        target_link_libraries(tests_core ws2_32)
    endif ()
endif ()

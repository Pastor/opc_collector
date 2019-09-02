cmake_minimum_required(VERSION 3.2.1)
project(hidapi)

include_directories(${CMAKE_BINARY_DIR})

if (WIN32)
    set(SOURCES hid_windows.c)
    set(LIBRARIES setupapi)
elseif (APPLE)
    set(SOURCES hid_macosx.c)

    find_library(CoreFoundation CoreFoundation)
    find_library(IOKit IOKit)
    set(LIBRARIES ${CoreFoundation} ${IOKit})
else ()
    set(SOURCES hid_linux.c)
endif ()

add_library(hidapi STATIC ${SOURCES})
target_compile_definitions(hidapi PRIVATE -DBUILD_LIBRARY -DCJSON_HIDE_SYMBOLS)
if (CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(hidapi PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -g -fPIC")
endif (CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(hidapi PUBLIC ${LIBRARIES})
add_library(Collector::HidApi ALIAS hidapi)

set_target_properties(hidapi
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (BUILD_TESTING)
    add_executable(hidtest hidtest.c)
    target_link_libraries(hidtest hidapi ${LIBRARIES})
endif ()

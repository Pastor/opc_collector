cmake_minimum_required(VERSION 3.2.1)
project(tests_base)

include_directories(${CMAKE_BINARY_DIR})

add_library(tests_base STATIC tests_base.cpp tests_base.hxx)
if (CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(tests_base PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -g -fPIC")
endif (CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(tests_base PUBLIC Collector::HidApi open62541 gtest)
add_library(Collector::Test ALIAS tests_base)

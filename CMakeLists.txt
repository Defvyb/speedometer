cmake_minimum_required(VERSION 3.8)

project(arrival_test_task)
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(commonLib)
add_subdirectory(speedometer)
add_subdirectory(generator)

find_package(GTest)
if(GTest_FOUND)
    enable_testing()
    message("GTest found, tests will be compiled")
    add_subdirectory(tests)
endif()

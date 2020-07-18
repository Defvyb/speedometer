cmake_minimum_required(VERSION 3.8)
project(commonLib)

file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/sources/*.cpp)

add_library(${PROJECT_NAME} ${headers} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            )

set(CMAKE_CXX_FLAGS "-Ofast")

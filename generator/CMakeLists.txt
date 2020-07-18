cmake_minimum_required(VERSION 3.8)
project(generator)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/sources/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} commonLib)

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            )

set(CMAKE_CXX_FLAGS "-Ofast")

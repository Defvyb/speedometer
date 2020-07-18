cmake_minimum_required(VERSION 3.17)
project(speedometer)

file(GLOB_RECURSE headers_hpp ${PROJECT_SOURCE_DIR}/sources/*.hpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/sources/*.h)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/sources/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/sources)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${headers} ${sources} ${headers_hpp})

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_ttf pthread)

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            )

set(PROJECT_LIB ${PROJECT_NAME}_lib)
set(CMAKE_CXX_FLAGS "-Ofast")

find_package(GTest)
if(GTest_FOUND)
    enable_testing()
    message("GTest found, tests will be compiled")
    add_subdirectory(test)
endif()

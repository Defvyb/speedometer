cmake_minimum_required(VERSION 3.8)
project(speedometer)

file(GLOB_RECURSE headers_hpp ${PROJECT_SOURCE_DIR}/sources/*.hpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/sources/*.h)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/sources/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/sources)
include_directories(commonLib)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${headers} ${sources} ${headers_hpp})

configure_file(resources/arrow.png  arrow.png COPYONLY)
configure_file(resources/speedometer.jpg  speedometer.jpg COPYONLY)
configure_file(resources/sdf.ttf  sdf.ttf COPYONLY)


target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_ttf pthread commonLib)

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            )

set(CMAKE_CXX_FLAGS "-Ofast")

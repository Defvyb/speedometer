project(tests)

include_directories(${GTEST_INCLUDE_DIR})


add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})


add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)


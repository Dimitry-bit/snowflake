# Search for .cpp files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME}_testbed ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}_testbed PRIVATE snowflake)

file(GLOB_RECURSE TESTS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_executable(${PROJECT_NAME}_tests ${TESTS_SRC_FILES})
target_link_libraries(${PROJECT_NAME}_tests PRIVATE snowflake Catch2::Catch2)

add_subdirectory(vendor)
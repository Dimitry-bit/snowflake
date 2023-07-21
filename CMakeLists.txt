cmake_minimum_required(VERSION 3.25)
project(snowflake)
set(CMAKE_CXX_STANDARD 17)

# Setup build directory
math(EXPR platform_bits "${CMAKE_SIZEOF_VOID_P} * 8")
set(project_platform_binary_dir ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}_testbed/$<CONFIG>-x${platform_bits})
# Set .dll/.exe files directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${project_platform_binary_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${project_platform_binary_dir})

# Setup cache directory
set(CMAKE_BUILD_FILES_DIRECTORY ${CMAKE_SOURCE_DIR}/intermediate/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})
set(CMAKE_BUILD_DIRECTORY ${CMAKE_SOURCE_DIR}/intermediate/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})
set(CMAKE_CACHEFILE_DIR ${CMAKE_SOURCE_DIR}/intermediate/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}_testbed/src)

add_subdirectory(snowflake)
add_subdirectory(snowflake_testbed)
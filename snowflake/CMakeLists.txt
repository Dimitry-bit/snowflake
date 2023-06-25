# Search for .cpp files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(snowflake SHARED ${SRC_FILES})
set_target_properties(snowflake PROPERTIES PREFIX "")
target_link_libraries(snowflake INTERFACE ${OPENGL_LIBRARY} glfw glew32s)
target_include_directories(snowflake INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(snowflake PRIVATE
        -Wall -Wextra -Wpedantic -Wuninitialized -Werror -Werror=pointer-arith
        -Wdouble-promotion -Wfloat-equal
        -Wformat=2 -Wformat-truncation -Wformat-overflow
        -Wundef -Wshadow -Wno-unused-variable -Wunused-label -Wno-array-bounds -Wno-infinite-recursion)
target_compile_options(snowflake PUBLIC -D ASSERTION_ENABLED)
target_compile_options(snowflake PUBLIC "$<$<CONFIG:RELEASE>:-D SNOWFLAKE_RELEASE>")
target_compile_options(snowflake PUBLIC "$<$<CONFIG:DEBUG>:-D SNOWFLAKE_DEBUG>")

add_subdirectory(vendor)
# Search for .cpp files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(snowflake SHARED ${SRC_FILES})
set_target_properties(snowflake PROPERTIES
        PREFIX ""
        DEBUG_POSTFIX "-d"
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
target_link_libraries(snowflake INTERFACE ${OPENGL_LIBRARY} glfw glew32s)
target_include_directories(snowflake INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(snowflake PRIVATE
        -Wall -Wextra -Wpedantic -Wuninitialized -Werror=pointer-arith
        -Wno-gnu-zero-variadic-macro-arguments
        -Wdouble-promotion -Wfloat-equal
        -Wundef -Wshadow -Wno-unused-variable -Wunused-label -Wno-array-bounds -Wno-infinite-recursion)

target_compile_options(snowflake PRIVATE
        -DSNOWFLAKE_EXPORT
        -DASSERTION_ENABLED
        "$<$<CONFIG:RELEASE>:-DSNOWFLAKE_RELEASE>"
        "$<$<CONFIG:DEBUG>:-DSNOWFLAKE_DEBUG>")

add_subdirectory(vendor)
include(FetchContent)
set(FETCHCONTENT_QUIET NO)

find_package(OpenGL REQUIRED)

if (NOT glfw_FOUND)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY "https://github.com/glfw/glfw.git"
            GIT_TAG "master"
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GLFW
    )

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)
endif ()

if (NOT glew_FOUND)
    FetchContent_Declare(
            glew
            URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip"
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GLEW
    )

    FetchContent_MakeAvailable(glew)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GLEW/include)
    set(GLEW_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/GLEW/src/glew.c)
    add_library(glew32s STATIC ${GLEW_SOURCE})
    target_compile_options(glew32s PUBLIC -DGLEW_STATIC)
    target_link_libraries(glew32s PRIVATE ${OPENGL_LIBRARY} ${EXTRA_LIBS})
    target_include_directories(glew32s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/GLEW/include)
endif ()
include(FetchContent)
set(FETCHCONTENT_QUIET NO)

if (NOT catch2_FOUND)
    FetchContent_Declare(
            catch2
            GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
            GIT_TAG "v3.4.0"
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch2
    )

    FetchContent_MakeAvailable(catch2)
endif ()
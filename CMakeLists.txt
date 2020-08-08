cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ObstacleRun
        VERSION 0.0.1
        DESCRIPTION "A 2D multiplayer game built with SDL2."
        HOMEPAGE_URL "https://github.com/kyranet/obstacle-run"
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(MSVC)
    # CMake by default appends /W3, so we need to strip it to prevent warnings (D9025)
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(assets)

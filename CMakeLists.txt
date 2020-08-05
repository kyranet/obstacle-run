cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ObstacleRun
        VERSION 0.0.1
        DESCRIPTION "A 2D multiplayer game built with SDL2."
        HOMEPAGE_URL "https://github.com/kyranet/obstacle-run"
        LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
message("CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_WARNING_LEVEL 4)

    # Replace the default /W flag to /W4 for higher verbosity.
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()

    # Workaround for MSVC development libraries in the University's laboratories, where access to
    # C:\ or C:\MinGW are forbidden. This serves to let anybody compile under restricted access.
    if (NOT BUILD_LOCAL)
        if (EXISTS "${CMAKE_SOURCE_DIR}/deps" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/deps")
            set(__DEPS_FOLDER 1)
        else ()
            set(__DEPS_FOLDER 0)
        endif ()

        set(BUILD_LOCAL "${__DEPS_FOLDER}" CACHE PATH "Assign this to 1 to use a local /deps/ folder.")
        unset(__DEPS_FOLDER)
    endif (NOT BUILD_LOCAL)
endif ()

# Print the compiler flags for debugging purposes.
message("CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")

add_subdirectory(src)
add_subdirectory(assets)

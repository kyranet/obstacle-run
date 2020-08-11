# Workaround for MSVC development libraries in the University's laboratories, where access to
# C:\ or C:\MinGW are forbidden. This serves to let anybody compile under restricted access.
if (NOT DEFINED BUILD_LOCAL)
  if (MSVC AND EXISTS "${CMAKE_SOURCE_DIR}/deps" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/deps")
    set(__DEPS_FOLDER ON)
  else ()
    set(__DEPS_FOLDER OFF)
  endif ()

  set(BUILD_LOCAL "${__DEPS_FOLDER}" CACHE PATH "Assign this to ON to use a local /deps/ folder.")
  unset(__DEPS_FOLDER)
endif ()

# If the generator is MSVC and a deps folder exists, try to link the libraries.
if (BUILD_LOCAL)
  file(GLOB SDL2_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2-*")
  file(GLOB SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_image-*")
  file(GLOB SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_ttf-*")
  file(GLOB SDL2_MIXER_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_mixer-*")
  file(GLOB SDL2_NET_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_net-*")

  set(ENV{SDL2DIR} ${SDL2_DIR})
  set(ENV{SDL2IMAGEDIR} ${SDL2_IMAGE_DIR})
  set(ENV{SDL2MIXERDIR} ${SDL2_MIXER_DIR})
  set(ENV{SDL2NETDIR} ${SDL2_NET_DIR})
  set(ENV{SDL2TTFDIR} ${SDL2_TTF_DIR})

  file(GLOB_RECURSE DEPENDENCY_DLLS
    "${SDL2_TTF_DIR}/lib/x64/*.dll"
    "${SDL2_MIXER_DIR}/lib/x64/*.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/*.dll"
    "${SDL2_NET_DIR}/lib/x64/*.dll"
    "${SDL2_DIR}/lib/x64/*.dll"
    )

  foreach (dll ${DEPENDENCY_DLLS})
    get_filename_component(dll_fn ${dll} NAME)
    configure_file(${dll} dll/${dll_fn} COPYONLY)
  endforeach ()
endif ()

option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
option(BOX2D_BUILD_DOCS "Build the Box2D documentation" OFF)
add_subdirectory(box2d)

add_executable(Dependencies IMPORTED GLOBAL)
set_target_properties(Dependencies PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/DO_NOT_USE)
set_target_properties(Dependencies PROPERTIES BUILD_LOCAL ${BUILD_LOCAL})

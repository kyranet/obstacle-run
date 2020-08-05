message("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

# If the generator is MSVC and a deps folder exists, try to link the libraries.
if (BUILD_LOCAL EQUAL 1)
  file(GLOB SDL2_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2-*")
  file(GLOB SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_image-*")
  file(GLOB SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_ttf-*")
  file(GLOB SDL2_MIXER_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_mixer-*")
  file(GLOB SDL2_NET_DIR "${CMAKE_SOURCE_DIR}/deps/SDL2_net-*")

  message("SDL2_DIR = ${SDL2_DIR}")
  message("SDL2_IMAGE_DIR = ${SDL2_IMAGE_DIR}")
  message("SDL2_TTF_DIR = ${SDL2_TTF_DIR}")
  message("SDL2_MIXER_DIR = ${SDL2_MIXER_DIR}")
  message("SDL2_NET_DIR = ${SDL2_NET_DIR}")
endif ()

# Find all SDL2 libraries.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_NET_INCLUDE_DIR} third-party/jsoncpp)

# If a deps folder was used, it is most likely that OS' scoped libraries will be tried to be used even if they are not
# available, so we copy the .dll files.
if (BUILD_LOCAL EQUAL 1)
  file(GLOB_RECURSE DEPENDENCY_DLLS
    "${SDL2_TTF_DIR}/*.dll"
    "${SDL2_MIXER_DIR}/*.dll"
    "${SDL2_IMAGE_DIR}/*.dll"
    "${SDL2_NET_DIR}/*.dll"
    "${SDL2_DIR}/*.dll"
    )
  file(COPY ${DEPENDENCY_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/src")
endif ()

set(GAME_SOURCES
  main.cpp
  Game.cpp
  scenes/Scene.cpp
  objects/GameObject.cpp
  managers/Input.cpp
  managers/SceneManager.cpp
  utils/TimePool.cpp
  third-party/jsoncpp/jsoncpp.cpp
  managers/ComponentManager.cpp
  components/Transform.cpp
  factories/TransformFactory.cpp
  components/Renderer.cpp
  factories/RendererFactory.cpp)

add_executable(Game ${GAME_SOURCES})
target_compile_features(Game PUBLIC cxx_std_17)
target_include_directories(Game
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )

# Link the libraries and install them.
target_link_libraries(Game ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_NET_LIBRARY})

add_custom_command(
  TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  $<TARGET_FILE_DIR:Assets>
  $<TARGET_FILE_DIR:Game>/assets
)
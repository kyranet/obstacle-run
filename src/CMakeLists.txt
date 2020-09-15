# Copyright 2020 Matt Jones and Contributors.
# Recipe based on https://github.com/novelrt/NovelRT

# Find all SDL2 libraries.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_NET_INCLUDE_DIR} third-party/jsoncpp)

set(DEPENDENCY_LIBRARIES box2d)

set(GAME_SOURCES
  components/BulletBox.cpp
  components/Button.cpp
  components/ImageRenderer.cpp
  components/NetworkController.cpp
  components/PhysicsBody.cpp
  components/PlayerController.cpp
  components/SolidRenderer.cpp
  components/TextRenderer.cpp
  components/Transform.cpp
  factories/BulletBoxFactory.cpp
  factories/ButtonFactory.cpp
  factories/ImageRendererFactory.cpp
  factories/NetworkControllerFactory.cpp
  factories/PhysicsBodyFactory.cpp
  factories/PlayerControllerFactory.cpp
  factories/SolidRendererFactory.cpp
  factories/TextRendererFactory.cpp
  factories/TransformFactory.cpp
  listeners/ContactListener.cpp
  Game.cpp
  main.cpp
  managers/ComponentManager.cpp
  managers/FontManager.cpp
  managers/ImageManager.cpp
  managers/Input.cpp
  managers/SceneManager.cpp
  networking/Client.cpp
  networking/Server.cpp
  objects/Component.cpp
  objects/Font.cpp
  objects/GameObject.cpp
  objects/Image.cpp
  scenes/Scene.cpp
  utils/Time.cpp
  third-party/jsoncpp/jsoncpp.cpp)

add_executable(Game ${GAME_SOURCES})
target_compile_features(Game PUBLIC cxx_std_17)
target_include_directories(Game
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/deps/box2d/include>
  $<INSTALL_INTERFACE:include>
  )

# Link the libraries and install them.
target_link_libraries(Game ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_NET_LIBRARY} ${DEPENDENCY_LIBRARIES})

add_custom_command(
  TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  $<TARGET_FILE_DIR:Assets>
  $<TARGET_FILE_DIR:Game>/assets
)

if (MSVC)
    add_custom_command(
      TARGET Game POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      $<TARGET_FILE_DIR:Dependencies>/dll
      $<TARGET_FILE_DIR:Game>
    )
endif ()

if (MSVC)
    target_compile_options(Game
      PRIVATE
      /W4
      /WX
      )
    get_target_property(opts Game COMPILE_OPTIONS)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Game
      PRIVATE
      -pedantic
      -pedantic-errors
      -Wall
      -Wextra
      -Werror
      -Wno-float-equal
      -Wno-padded
      )
else ()
    target_compile_options(Game
      PRIVATE
      -pedantic
      -pedantic-errors
      -Wall
      -Wextra
      -Werror
      -Wconversion
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-float-equal
      -Wno-padded
      -Wno-reserved-id-macro
      )
endif ()

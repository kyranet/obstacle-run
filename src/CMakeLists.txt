# Find all SDL2 libraries.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_NET_INCLUDE_DIR} third-party/jsoncpp)

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

get_target_property(opts Dependencies BUILD_LOCAL)
if (BUILD_LOCAL)
  add_custom_command(
    TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:Dependencies>
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
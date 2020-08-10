# Copyright 2020 Matt Jones and Contributors.
# Recipe based on https://github.com/novelrt/NovelRT

set(ASSET_FILES
  fonts/all.json
  fonts/PaperWorks.ttf
  images/all.json
  images/ForestBackground.jpg
  scenes/menu.json)

foreach(resource ${ASSET_FILES})
    configure_file(${resource} assets/${resource} COPYONLY)
endforeach()

add_executable(Assets IMPORTED GLOBAL)
set_target_properties(Assets PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/assets/DO_NOT_USE)
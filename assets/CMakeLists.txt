set(ASSET_FILES
        scenes/menu.json)

foreach(resource ${ASSET_FILES})
    configure_file(${resource} assets/${resource} COPYONLY)
endforeach()

add_executable(Assets IMPORTED GLOBAL)
set_target_properties(Assets PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/assets/IF_YOU_SEE_THIS_YOU_TRIED_TO_RUN_THE_RESOURCES_TARGET)
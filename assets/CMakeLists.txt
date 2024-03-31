file(GLOB ASSET_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

# Copy all asset files to the build directory
foreach(ASSET_FILE ${ASSET_FILES})
    configure_file(${ASSET_FILE} ${CMAKE_BINARY_DIR}/${ASSET_FILE} COPYONLY)
endforeach()

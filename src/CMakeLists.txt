# Set the source files
set(SRC_FILES
    main.c
)

# Add an executable target
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} sub::lib2)
target_link_libraries(${PROJECT_NAME} sub::lib1)
target_link_libraries(${PROJECT_NAME} raylib)



add_executable(Visualization
    "main.cpp"
    "Window.cpp"
    "App.cpp"
    "AppLogic.cpp"
    "Camera.cpp"
    "ShaderProgram.cpp"
    "Timer.cpp"
    "VoxelModel.cpp"
    "Vec3Table.cpp"
    "VoxelTable.cpp"
    "IsoSurface.cpp"
    "CrossSection.cpp"
)

target_link_libraries(Visualization
    OpenGL::GL
    glad
    glfw
    imgui
    glm
)

if(UNIX)
    add_custom_command(TARGET Visualization POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Visualization>/shaders)

    add_custom_command(TARGET Visualization  POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink  ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Visualization>/assets) 

    set(CMAKE_EXPORT_COMPILE_COMMANDS)
endif(UNIX)

set_property(TARGET Visualization PROPERTY CXX_STANDARD 20)

add_executable(nes)

target_sources(nes
    PRIVATE
    nes.cpp
)

target_link_libraries(nes
    PRIVATE
    imgui
    glfw
    OpenGL::GL
)
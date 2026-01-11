add_executable(nes)

target_compile_features(nes
    PRIVATE
    cxx_std_23
)

target_sources(nes
    PRIVATE
    nes.cpp
    system.cpp
)

target_link_libraries(nes
    PRIVATE
    imgui
    glfw
    OpenGL::GL
)
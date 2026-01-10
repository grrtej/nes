add_library(imgui)

target_sources(imgui
    PRIVATE
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_demo.cpp

    PUBLIC
    FILE_SET imgui
    TYPE HEADERS
    BASE_DIRS
    imgui/
    FILES
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imconfig.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
)

target_sources(imgui
    PRIVATE
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp

    PUBLIC
    FILE_SET imgui_backends
    TYPE HEADERS
    BASE_DIRS
    imgui/backends/
    FILES 
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.h
)

target_link_libraries(imgui
    PRIVATE
    glfw
)

add_subdirectory(glfw/)
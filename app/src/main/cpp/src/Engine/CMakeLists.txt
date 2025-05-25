add_library(Engine STATIC 
        engine.cpp
        renderer.cpp
        player.cpp
        camera.cpp
)

target_include_directories(Engine PUBLIC ../../include)

# Link OpenGL ES 2.0
target_link_libraries(Engine GLESv2)
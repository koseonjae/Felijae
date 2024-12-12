add_library(Engine STATIC
    include/Engine/Model/Camera.h
    include/Engine/Model/Light.h
    include/Engine/Model/Model.h
    include/Engine/Model/Scene.h
    Graphics/include/Graphics/Model/Uniforms.h
    include/Engine/Renderer/Renderer.h
    include/Engine/Renderer/ComputeRenderer.h
    include/Engine/Renderer/GraphicsRenderer.h
    include/Engine/Renderer/ForwardRenderer.h
    include/Engine/Engine.h

    src/Model/Camera.cpp
    src/Model/Model.cpp
    src/Model/Scene.cpp
    Graphics/src/Model/Uniforms.cpp
    src/Renderer/ComputeRenderer.cpp
    src/Renderer/GraphicsRenderer.cpp
    src/Renderer/ForwardRenderer.cpp
    src/Engine.cpp)

target_include_directories(Engine PUBLIC
    include)

add_subdirectory(Base)
add_subdirectory(ShaderConverter)
add_subdirectory(Graphics)

target_link_libraries(Engine
    PUBLIC
    Graphics)

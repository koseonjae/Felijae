add_library(Engine STATIC
    include/Engine/Model/Camera.h
    include/Engine/Model/Light.h
    include/Engine/Model/Model.h
    include/Engine/Model/Scene.h
    include/Engine/Renderer/Renderer.h
    include/Engine/Renderer/ForwardRenderer.h
    include/Engine/Engine.h

    src/Model/Camera.cpp
    src/Model/Model.cpp
    src/Model/Scene.cpp
    src/Renderer/ForwardRenderer.cpp
    src/Engine.cpp)

target_include_directories(Engine PUBLIC
    include)

add_subdirectory(external)
add_subdirectory(Base)
add_subdirectory(Graphics)

target_link_libraries(Engine
    PUBLIC
    Graphics)

add_library(Engine STATIC
    Base/include/Base/Triangle.h
    Base/include/Base/Object.h
    include/Engine/Model/Camera.h
    include/Engine/Model/Light.h
    include/Engine/Model/Model.h
    include/Engine/Model/Scene.h
    include/Engine/Engine.h

    Base/src/Triangle.cpp
    Base/src/Object.cpp
    src/Model/Camera.cpp
    src/Model/Model.cpp
    src/Model/Scene.cpp
    src/Engine.cpp)

target_include_directories(Engine PUBLIC
    include)

add_subdirectory(external)
add_subdirectory(Base)
add_subdirectory(Graphics)

target_link_libraries(Engine
    PUBLIC
    Graphics)

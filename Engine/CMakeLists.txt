add_library(Engine STATIC
  include/Engine/Engine.h

  src/Engine.cpp)

target_include_directories(Engine PUBLIC
  include)

add_subdirectory(Graphics)

target_link_libraries(Engine
    PUBLIC
    Graphics)

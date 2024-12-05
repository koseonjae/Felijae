add_library(SDLWrapper STATIC
    include/SDLWrapper/SDLWrapper.h
    include/SDLWrapper/OpenGLSDLWrapper.h
    include/SDLWrapper/MetalSDLWrapper.h

    src/SDLWrapper.cpp
    src/OpenGLSDLWrapper.cpp
    src/MetalSDLWrapper.cpp)

find_package(SDL2 CONFIG REQUIRED)

target_include_directories(SDLWrapper PUBLIC include)

target_link_libraries(SDLWrapper PRIVATE
    MetalCpp
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
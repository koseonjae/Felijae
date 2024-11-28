add_library(Base STATIC
    include/Base/ImageFormat.h
    include/Base/ImageData.h
    include/Base/Object/Object.h
    include/Base/Object/Polygons.h
    include/Base/Utility/FileReader.h
    include/Base/Utility/ImageLoader.h
    include/Base/Utility/ThreadChecker.h
    include/Base/File/File.h

    src/Object/Polygons.cpp
    src/Object/Object.cpp
    src/Utility/FileReader.cpp
    src/Utility/ImageLoader.cpp
    src/Utility/ThreadChecker.cpp
    src/File/File.cpp)

find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

target_include_directories(Base PUBLIC
    include)

target_link_libraries(Base PUBLIC
    stb
    glm::glm
    tinyobjloader::tinyobjloader)

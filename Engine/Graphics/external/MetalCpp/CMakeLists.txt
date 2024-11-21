add_library(MetalCpp STATIC src/impl.cpp)

target_include_directories(MetalCpp PUBLIC
    include
    metal-cpp)

# Metal
find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
find_library(APPLE_FWK_METAL Metal REQUIRED)

target_link_libraries(MetalCpp
    PUBLIC
    ${APPLE_FWK_FOUNDATION}
    ${APPLE_FWK_QUARTZ_CORE}
    ${APPLE_FWK_METAL})

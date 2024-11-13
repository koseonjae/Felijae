add_library(metal STATIC src/impl.cpp)

target_include_directories(metal PUBLIC
    include
    metal-cpp)

# Metal
find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
find_library(APPLE_FWK_METAL Metal REQUIRED)

target_link_libraries(metal
    PUBLIC
    ${APPLE_FWK_FOUNDATION}
    ${APPLE_FWK_QUARTZ_CORE}
    ${APPLE_FWK_METAL})

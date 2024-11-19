#include <metal_stdlib>

using namespace metal;

struct RasterizerData {
    float4 position [[position]];
};

fragment float4 fragmentShader(RasterizerData in [[stage_in]]) {
    return float4(1.0, 0.0, 0.0, 1.0);
}

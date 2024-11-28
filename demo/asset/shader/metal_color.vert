#include <metal_stdlib>

using namespace metal;

struct AAPLVertex {
    float3 position [[attribute(0)]];
    float3 normal [[attribute(1)]];
    float2 texCoord [[attribute(2)]];
};

struct RasterizerData {
    float4 position [[position]];
};

vertex RasterizerData vertexShader(AAPLVertex v [[stage_in]]) {
    RasterizerData out;
    out.position = float4(v.position.xyz, 1.0);
    return out;
}
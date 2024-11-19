#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

enum AAPLVertexInputIndex
{
  AAPLVertexInputIndexVertices     = 0,
  AAPLVertexInputIndexViewportSize = 1,
};

struct AAPLVertex
{
  float3 position [[attribute(0)]];
  float3 normal [[attribute(1)]];
  float2 texCoord [[attribute(2)]];
};

struct RasterizerData
{
    float4 position [[position]];

};

vertex RasterizerData vertexShader(AAPLVertex v [[stage_in]]) {
    RasterizerData out;
    out.position = float4(v.position.xyz, 1.0);
    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]]) {
    return float4(1.0, 0.0, 0.0, 1.0);
}

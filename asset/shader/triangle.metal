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
  vector_float2 position [[attribute(0)]];
  vector_float4 color [[attribute(1)]];
};

struct RasterizerData
{
    float4 position [[position]];
    float4 color;
};

vertex RasterizerData vertexShader(AAPLVertex v [[stage_in]]) {
    RasterizerData out;
    out.position = float4(v.position.xy, 0.0, 1.0);
    out.color = v.color;
    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]]) {
    return in.color;
}

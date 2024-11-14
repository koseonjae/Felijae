#pragma once

#include <simd/simd.h>

enum AAPLVertexInputIndex
{
  AAPLVertexInputIndexVertices     = 0,
  AAPLVertexInputIndexViewportSize = 1,
};

struct AAPLVertex
{
  vector_float2 position;
  vector_float4 color;
};

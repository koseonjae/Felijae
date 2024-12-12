#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 v_worldPos;

layout(set = 0, binding = 0) uniform Uniforms {
    mat4 uWorldMat;
    mat4 uViewMat;
    mat4 uProjMat;
} uniforms;

void main() {
    // Transform position to clip space
    gl_Position = uniforms.uProjMat * uniforms.uViewMat * uniforms.uWorldMat * vec4(position, 1.0);

    v_worldPos = (uniforms.uWorldMat * vec4(position, 1.0)).xyz;
}

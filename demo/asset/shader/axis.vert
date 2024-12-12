#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 v_normal;
layout(location = 1) out vec2 v_texCoord;
layout(location = 2) out vec3 v_viewDir;
layout(location = 3) out vec3 v_worldPos;

layout(set = 0, binding = 0) uniform Uniforms {
    mat4 uWorldMat;
    mat4 uViewMat;
    mat4 uProjMat;
    vec3 uCameraPosition;
    vec3 uLightDir;
    vec3 uLightColor;
    vec3 uEmitLight;
} uniforms;

void main() {
    // Transform position to clip space
    gl_Position = uniforms.uProjMat * uniforms.uViewMat * uniforms.uWorldMat * vec4(position, 1.0);

    // Pass texture coordinates to fragment shader
    v_texCoord = texCoord;

    // Calculate normal matrix and transform normal
    mat3 normalMatrix = transpose(inverse(mat3(uniforms.uWorldMat)));
    v_normal = normalize(normalMatrix * normal);

    // Calculate view direction
    vec3 worldPos = (uniforms.uWorldMat * vec4(position, 1.0)).xyz;
    v_worldPos = worldPos;
    v_viewDir = normalize(uniforms.uCameraPosition - worldPos);
}

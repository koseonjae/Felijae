#version 330

struct Uniforms
{
    mat4 uWorldMat;
    mat4 uViewMat;
    mat4 uProjMat;
    vec3 uCameraPosition;
    vec3 uLightDir;
    vec3 uLightColor;
    vec3 uEmitLight;
};

uniform Uniforms uniforms;

layout(location = 0) in vec3 position;
out vec2 v_texCoord;
layout(location = 2) in vec2 texCoord;
out vec3 v_normal;
layout(location = 1) in vec3 normal;
out vec3 v_viewDir;

void main()
{
    gl_Position = ((uniforms.uProjMat * uniforms.uViewMat) * uniforms.uWorldMat) * vec4(position, 1.0);
    v_texCoord = texCoord;
    mat3 normalMatrix = transpose(inverse(mat3(uniforms.uWorldMat[0].xyz, uniforms.uWorldMat[1].xyz, uniforms.uWorldMat[2].xyz)));
    v_normal = normalize(normalMatrix * normal);
    vec3 worldPos = (uniforms.uWorldMat * vec4(position, 1.0)).xyz;
    v_viewDir = normalize(uniforms.uCameraPosition - worldPos);
}
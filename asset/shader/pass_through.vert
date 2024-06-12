#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 vTexCoord;

void main() {
    vTexCoord = texCoord;
    gl_Position = projMat * viewMat * worldMat * vec4(pos, 1.0);
}
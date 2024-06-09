#version 330

layout(location = 0) in vec3 pos;

uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    gl_Position = projMat * viewMat * worldMat * vec4(pos, 1.0);
}
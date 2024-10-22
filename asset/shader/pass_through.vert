#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uWorldMat;
uniform mat4 uViewMat;
uniform mat4 uProjMat;

out vec2 vTexCoord;

void main() {
    vTexCoord = texCoord;
    gl_Position = uProjMat * uViewMat * uWorldMat * vec4(pos, 1.0);
}
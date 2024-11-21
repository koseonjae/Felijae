#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uWorldMat;
uniform mat4 uViewMat;
uniform mat4 uProjMat;
uniform vec3 uCameraPosition;

out vec3 v_normal; // world normal
out vec2 v_texCoord; // bypass
out vec3 v_viewDir; // view Vector

void main() {
    // gl_Position
    // world position
    // world normal vector
    // world view vector
    gl_Position = uProjMat * uViewMat * uWorldMat * vec4(position, 1.0);
    v_texCoord = texCoord;
    v_normal = normalize(transpose(inverse(mat3(uWorldMat))) * normal);
    vec3 worldPos = (uWorldMat * vec4(position, 1.0)).xyz;
    v_viewDir = normalize(uCameraPosition - worldPos);
}
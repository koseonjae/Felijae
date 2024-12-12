#version 450

layout(location = 0) in vec3 v_worldPos;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform Uniforms {
    mat4 uWorldMat;
    mat4 uViewMat;
    mat4 uProjMat;
} uniforms;

void main() {
    vec3 pos = v_worldPos;
    vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
    if (pos.x > pos.y && pos.x > pos.z)
        diffuseColor = vec3(1.0, 0.0, 0.0);
    else if (pos.y > pos.x && pos.y > pos.z)
        diffuseColor = vec3(0.0, 1.0, 0.0);
    else
        diffuseColor = vec3(0.0, 0.0, 1.0);
    fragColor = vec4(diffuseColor, 1.0);
}

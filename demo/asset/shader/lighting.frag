#version 450

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec3 v_viewDir;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform Uniforms {
    mat4 uWorldMat;
    mat4 uViewMat;
    mat4 uProjMat;
    vec3 uCameraPosition;
    vec3 uLightDir;
    vec3 uLightColor;
    vec3 uEmitLight;
} uniforms;

layout(set = 0, binding = 1) uniform sampler2D uTexture;

void main() {
    // Normalize inputs
    vec3 normal = normalize(v_normal);
    vec3 view = normalize(v_viewDir);
    vec3 light = normalize(uniforms.uLightDir);
    vec3 reflectDir = normalize(reflect(-light, normal));

    // Sample the diffuse texture
    vec3 diffuseColor = texture(uTexture, v_texCoord).rgb;

    // Ambient lighting
    float ambientConstant = 0.1;
    vec3 ambient = ambientConstant * diffuseColor;

    // Diffuse lighting
    vec3 diffuse = max(dot(light, normal), 0.0) * diffuseColor;

    // Specular lighting
    float specularShiness = 10.0;
    vec3 specular = pow(max(dot(reflectDir, view), 0.0), specularShiness) * uniforms.uLightColor;

    // Emissive lighting
    vec3 emit = uniforms.uEmitLight;

    // Final color
    vec3 color = ambient + diffuse + specular + emit;

    fragColor = vec4(color, 1.0);
}

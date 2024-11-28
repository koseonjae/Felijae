#version 330

in vec3 v_normal; // world normal
in vec2 v_texCoord; // bypass
in vec3 v_viewDir; // view Vector

uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uEmitLight;
uniform sampler2D uTexture;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 normal = normalize(v_normal);
    vec3 view = normalize(v_viewDir);
    vec3 light = normalize(uLightDir);
    vec3 reflectDir = normalize(reflect(light, normal));

    vec3 diffuseColor = texture(uTexture, v_texCoord).rgb;

    // ambient
    float ambientConstant = 0.1; // todo: uniform
    vec3 ambient = ambientConstant * diffuseColor;

    // diffuse
    vec3 diffuse = dot(uLightDir, v_normal) * diffuseColor;

    // specular
    float specularShiness = 10; // todo: uniform
    vec3 specular = pow(max(dot(reflectDir, light), 0), specularShiness) * uLightColor;

    // emissive
    vec3 emit = uEmitLight;

    vec3 color = ambient + diffuseColor + specular + emit;

    fragColor = vec4(color, 1.0);
}